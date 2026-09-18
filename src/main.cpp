#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/file.hpp>

#include <mpg123.h>

#include <filesystem>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>

using namespace geode::prelude;


// ============================================================
// RESULTADO DEL ANALISIS BASICO
// ============================================================

struct AudioInfo {
    long sampleRate = 0;
    int channels = 0;
    int encoding = 0;

    long long totalSamples = 0;

    double duration = 0.0;

    double rms = 0.0;
    double peak = 0.0;
};


// ============================================================
// DECODIFICADOR MPG123
// ============================================================

static bool decodeMP3(
    const std::filesystem::path& path,
    AudioInfo& info
) {
    int error = MPG123_OK;

    mpg123_handle* handle = mpg123_new(nullptr, &error);

    if (!handle) {
        log::error("MPG123: no se pudo crear el decoder");
        return false;
    }

    // Queremos PCM signed 16-bit.
    if (mpg123_format_none(handle) != MPG123_OK) {
        log::error("MPG123: error configurando formato");
        mpg123_delete(handle);
        return false;
    }

    int openResult = mpg123_open(
        handle,
        path.string().c_str()
    );

    if (openResult != MPG123_OK) {
        log::error(
            "MPG123: no se pudo abrir MP3: {}",
            mpg123_strerror(handle)
        );

        mpg123_delete(handle);
        return false;
    }

    long rate = 0;
    int channels = 0;
    int encoding = 0;

    if (mpg123_getformat(
        handle,
        &rate,
        &channels,
        &encoding
    ) != MPG123_OK) {

        log::error("MPG123: no se pudo obtener el formato");

        mpg123_close(handle);
        mpg123_delete(handle);

        return false;
    }

    info.sampleRate = rate;
    info.channels = channels;
    info.encoding = encoding;

    // --------------------------------------------------------
    // Decodificar PCM
    // --------------------------------------------------------

    const size_t BUFFER_SIZE = 16384;

    unsigned char buffer[BUFFER_SIZE];

    std::vector<int16_t> samples;

    samples.reserve(
        static_cast<size_t>(rate) *
        static_cast<size_t>(channels) *
        60
    );

    size_t bytesRead = 0;

    while (true) {

        int result = mpg123_read(
            handle,
            buffer,
            BUFFER_SIZE,
            &bytesRead
        );

        if (bytesRead > 0) {

            size_t sampleCount =
                bytesRead / sizeof(int16_t);

            const int16_t* pcm =
                reinterpret_cast<const int16_t*>(buffer);

            samples.insert(
                samples.end(),
                pcm,
                pcm + sampleCount
            );
        }

        if (result == MPG123_DONE) {
            break;
        }

        if (result != MPG123_OK) {

            if (result == MPG123_NEW_FORMAT) {
                continue;
            }

            log::error(
                "MPG123: error durante decodificacion: {}",
                mpg123_strerror(handle)
            );

            mpg123_close(handle);
            mpg123_delete(handle);

            return false;
        }
    }

    mpg123_close(handle);
    mpg123_delete(handle);

    if (samples.empty()) {
        log::error("MPG123: no se obtuvieron muestras PCM");
        return false;
    }

    // --------------------------------------------------------
    // Estadisticas basicas
    // --------------------------------------------------------

    info.totalSamples =
        static_cast<long long>(
            samples.size()
        );

    if (rate > 0 && channels > 0) {

        info.duration =
            static_cast<double>(
                samples.size()
            )
            /
            static_cast<double>(
                rate * channels
            );
    }

    double sumSquares = 0.0;
    double peak = 0.0;

    for (int16_t sample : samples) {

        double normalized =
            static_cast<double>(sample)
            /
            32768.0;

        double absolute =
            std::abs(normalized);

        sumSquares +=
            normalized * normalized;

        if (absolute > peak)
            peak = absolute;
    }

    info.rms =
        std::sqrt(
            sumSquares /
            static_cast<double>(samples.size())
        );

    info.peak = peak;

    log::info(
        "======================================"
    );

    log::info(
        "AI LEVEL GENERATOR - MPG123"
    );

    log::info(
        "Sample Rate: {} Hz",
        info.sampleRate
    );

    log::info(
        "Channels: {}",
        info.channels
    );

    log::info(
        "Duration: {:.2f} sec",
        info.duration
    );

    log::info(
        "Samples: {}",
        info.totalSamples
    );

    log::info(
        "RMS: {:.5f}",
        info.rms
    );

    log::info(
        "Peak: {:.5f}",
        info.peak
    );

    log::info(
        "======================================"
    );

    return true;
}


// ============================================================
// PANEL
// ============================================================

class AIGeneratorPanel : public CCLayer {

protected:

    CCLabelBMFont* m_musicName = nullptr;
    CCLabelBMFont* m_analysisStatus = nullptr;
    CCLabelBMFont* m_generationStatus = nullptr;

    std::filesystem::path m_musicPath;

    bool m_hasMusic = false;
    bool m_hasAnalysis = false;
    bool m_isAnalyzing = false;


    // --------------------------------------------------------
    // LABEL
    // --------------------------------------------------------

    CCLabelBMFont* makeLabel(
        const char* text,
        float size
    ) {

        auto label =
            CCLabelBMFont::create(
                text,
                "bigFont.fnt"
            );

        if (!label)
            return nullptr;

        label->setScale(size / 30.f);

        return label;
    }


    // --------------------------------------------------------
    // BUTTON
    // --------------------------------------------------------

    CCMenuItemSpriteExtra* makeButton(
        const char* text,
        const char* callback
    ) {

        auto sprite =
            ButtonSprite::create(
                text,
                100,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                30.f,
                1.f
            );

        auto button =
            CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(
                    AIGeneratorPanel::onAddMusic
                )
            );

        return button;
    }


    // --------------------------------------------------------
    // CARD
    // --------------------------------------------------------

    CCLayerColor* makeCard(
        float width,
        float height
    ) {

        auto card =
            CCLayerColor::create(
                ccc4(
                    8,
                    25,
                    48,
                    255
                )
            );

        card->setContentSize(
            CCSize(width, height)
        );

        return card;
    }


    // --------------------------------------------------------
    // INIT
    // --------------------------------------------------------

    bool init() {

        if (!CCLayer::init())
            return false;

        auto screen =
            CCDirector::sharedDirector()
                ->getWinSize();

        this->setContentSize(screen);

        // ====================================================
        // FONDO
        // ====================================================

        auto overlay =
            CCLayerColor::create(
                ccc4(
                    0,
                    5,
                    15,
                    190
                )
            );

        overlay->setContentSize(screen);

        this->addChild(
            overlay,
            -10
        );


        // ====================================================
        // PANEL
        // ====================================================

        const float panelW = 540.f;
        const float panelH = 320.f;

        const float panelX =
            (screen.width - panelW) / 2.f;

        const float panelY =
            (screen.height - panelH) / 2.f;


        // Borde cyan

        auto border =
            CCLayerColor::create(
                ccc4(
                    0,
                    210,
                    255,
                    255
                )
            );

        border->setContentSize(
            CCSize(
                panelW + 4.f,
                panelH + 4.f
            )
        );

        border->setPosition(
            panelX - 2.f,
            panelY - 2.f
        );

        this->addChild(
            border
        );


        // Panel principal

        auto panel =
            CCLayerColor::create(
                ccc4(
                    7,
                    18,
                    38,
                    255
                )
            );

        panel->setContentSize(
            CCSize(
                panelW,
                panelH
            )
        );

        panel->setPosition(
            panelX,
            panelY
        );

        this->addChild(
            panel,
            1
        );


        // ====================================================
        // HEADER
        // ====================================================

        auto header =
            CCLayerColor::create(
                ccc4(
                    8,
                    35,
                    62,
                    255
                )
            );

        header->setContentSize(
            CCSize(
                panelW,
                58.f
            )
        );

        header->setPosition(
            panelX,
            panelY + panelH - 58.f
        );

        this->addChild(
            header,
            2
        );


        // ====================================================
        // TITULO
        // ====================================================

        auto title =
            CCLabelBMFont::create(
                "AI LEVEL GENERATOR",
                "bigFont.fnt"
            );

        title->setScale(
            0.75f
        );

        title->setPosition(
            panelX + panelW / 2.f,
            panelY + panelH - 25.f
        );

        this->addChild(
            title,
            3
        );


        // ====================================================
        // SUBTITULO
        // ====================================================

        auto subtitle =
            CCLabelBMFont::create(
                "CREA NIVELES CON INTELIGENCIA ARTIFICIAL",
                "chatFont.fnt"
            );

        subtitle->setScale(
            0.62f
        );

        subtitle->setPosition(
            panelX + panelW / 2.f,
            panelY + panelH - 47.f
        );

        this->addChild(
            subtitle,
            3
        );


        // ====================================================
        // TARJETA DE INFORMACION
        // ====================================================

        auto card =
            makeCard(
                500.f,
                105.f
            );

        card->setPosition(
            panelX + 20.f,
            panelY + 135.f
        );

        this->addChild(
            card,
            2
        );


        // MUSICA

        auto musicLabel =
            CCLabelBMFont::create(
                "MUSICA",
                "goldFont.fnt"
            );

        musicLabel->setScale(
            0.60f
        );

        musicLabel->setAnchorPoint(
            ccp(
                0.f,
                0.5f
            )
        );

        musicLabel->setPosition(
            panelX + 35.f,
            panelY + 212.f
        );

        this->addChild(
            musicLabel,
            3
        );


        m_musicName =
            CCLabelBMFont::create(
                "Ninguna musica seleccionada",
                "chatFont.fnt"
            );

        m_musicName->setScale(
            0.65f
        );

        m_musicName->setAnchorPoint(
            ccp(
                0.f,
                0.5f
            )
        );

        m_musicName->setPosition(
            panelX + 35.f,
            panelY + 190.f
        );

        this->addChild(
            m_musicName,
            3
        );


        // ANALISIS

        auto analysisLabel =
            CCLabelBMFont::create(
                "ANALISIS",
                "goldFont.fnt"
            );

        analysisLabel->setScale(
            0.60f
        );

        analysisLabel->setAnchorPoint(
            ccp(
                0.f,
                0.5f
            )
        );

        analysisLabel->setPosition(
            panelX + 270.f,
            panelY + 212.f
        );

        this->addChild(
            analysisLabel,
            3
        );


        m_analysisStatus =
            CCLabelBMFont::create(
                "Sin analisis",
                "chatFont.fnt"
            );

        m_analysisStatus->setScale(
            0.65f
        );

        m_analysisStatus->setAnchorPoint(
            ccp(
                0.f,
                0.5f
            )
        );

        m_analysisStatus->setPosition(
            panelX + 270.f,
            panelY + 190.f
        );

        this->addChild(
            m_analysisStatus,
            3
        );


        // GENERACION

        auto generationLabel =
            CCLabelBMFont::create(
                "GENERACION",
                "goldFont.fnt"
            );

        generationLabel->setScale(
            0.60f
        );

        generationLabel->setAnchorPoint(
            ccp(
                0.f,
                0.5f
            )
        );

        generationLabel->setPosition(
            panelX + 270.f,
            panelY + 162.f
        );

        this->addChild(
            generationLabel,
            3
        );


        m_generationStatus =
            CCLabelBMFont::create(
                "Sin generar",
                "chatFont.fnt"
            );

        m_generationStatus->setScale(
            0.65f
        );

        m_generationStatus->setAnchorPoint(
            ccp(
                0.f,
                0.5f
            )
        );

        m_generationStatus->setPosition(
            panelX + 270.f,
            panelY + 142.f
        );

        this->addChild(
            m_generationStatus,
            3
        );


        // ====================================================
        // BOTONES
        // ====================================================

        auto menu =
            CCMenu::create();

        menu->setPosition(
            panelX + 20.f,
            panelY + 82.f
        );

        this->addChild(
            menu,
            5
        );


        // AGREGAR

        auto addSprite =
            ButtonSprite::create(
                "AGREGAR",
                90,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                30.f,
                1.f
            );

        auto addButton =
            CCMenuItemSpriteExtra::create(
                addSprite,
                this,
                menu_selector(
                    AIGeneratorPanel::onAddMusic
                )
            );

        addButton->setPosition(
            85.f,
            0.f
        );

        menu->addChild(
            addButton
        );


        // ANALIZAR

        auto analyzeSprite =
            ButtonSprite::create(
                "ANALIZAR",
                90,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                30.f,
                1.f
            );

        auto analyzeButton =
            CCMenuItemSpriteExtra::create(
                analyzeSprite,
                this,
                menu_selector(
                    AIGeneratorPanel::onAnalyze
                )
            );

        analyzeButton->setPosition(
            220.f,
            0.f
        );

        menu->addChild(
            analyzeButton
        );


        // GENERAR

        auto generateSprite =
            ButtonSprite::create(
                "GENERAR",
                90,
                true,
                "goldFont.fnt",
                "GJ_button_04.png",
                30.f,
                1.f
            );

        auto generateButton =
            CCMenuItemSpriteExtra::create(
                generateSprite,
                this,
                menu_selector(
                    AIGeneratorPanel::onGenerate
                )
            );

        generateButton->setPosition(
            355.f,
            0.f
        );

        menu->addChild(
            generateButton
        );


        // ====================================================
        // CERRAR
        // ====================================================

        auto closeSprite =
            CCSprite::createWithSpriteFrameName(
                "GJ_closeBtn_001.png"
            );

        auto closeButton =
            CCMenuItemSpriteExtra::create(
                closeSprite,
                this,
                menu_selector(
                    AIGeneratorPanel::onClose
                )
            );

        closeButton->setPosition(
            panelX + panelW - 18.f,
            panelY + panelH - 18.f
        );

        auto closeMenu =
            CCMenu::create();

        closeMenu->setPosition(
            CCPointZero
        );

        closeMenu->addChild(
            closeButton
        );

        this->addChild(
            closeMenu,
            10
        );


        // ====================================================
        // FOOTER
        // ====================================================

        auto footer =
            CCLabelBMFont::create(
                "MP3  â€¢  ANALISIS MUSICAL  â€¢  GENERACION PROCEDURAL",
                "chatFont.fnt"
            );

        footer->setScale(
            0.55f
        );

        footer->setPosition(
            panelX + panelW / 2.f,
            panelY + 25.f
        );

        this->addChild(
            footer,
            3
        );


        return true;
    }


    // ========================================================
    // AGREGAR MUSICA
    // ========================================================

    void onAddMusic(CCObject*) {

        geode::utils::file::FilePickOptions options;

        geode::utils::file::FilePickOptions::Filter mp3Filter;

        mp3Filter.description =
            "Audio MP3";

        mp3Filter.files.insert(
            ".mp3"
        );

        options.filters.push_back(
            mp3Filter
        );


        geode::async::spawn(
            geode::utils::file::pick(
                geode::utils::file::PickMode::OpenFile,
                options
            ),

            [this](
                geode::utils::file::PickResult result
            ) {

                if (!result.isOk()) {

                    FLAlertLayer::create(
                        "AI LEVEL GENERATOR",
                        "No se selecciono ningun archivo.",
                        "OK"
                    )->show();

                    return;
                }

                auto selected =
                    result.unwrap();

                if (!selected)
                    return;


                m_musicPath =
                    selected.value();


                std::string extension =
                    m_musicPath.extension().string();

                std::transform(
                    extension.begin(),
                    extension.end(),
                    extension.begin(),

                    [](
                        unsigned char c
                    ) {

                        return static_cast<char>(
                            std::tolower(c)
                        );
                    }
                );


                if (extension != ".mp3") {

                    m_musicPath.clear();

                    m_hasMusic = false;

                    m_musicName->setString(
                        "Ninguna musica seleccionada"
                    );

                    FLAlertLayer::create(
                        "AI LEVEL GENERATOR",
                        "Solo se permiten archivos MP3.",
                        "OK"
                    )->show();

                    return;
                }


                std::string songName =
                    m_musicPath.stem().string();


                if (songName.empty())
                    songName =
                        m_musicPath.filename().string();


                m_hasMusic = true;

                m_hasAnalysis = false;


                m_analysisStatus->setString(
                    "Sin analisis"
                );

                m_generationStatus->setString(
                    "Sin generar"
                );

                m_musicName->setString(
                    songName.c_str()
                );


                float scale =
                    0.65f;

                if (songName.length() > 32)
                    scale = 0.50f;

                else if (songName.length() > 24)
                    scale = 0.56f;

                m_musicName->setScale(
                    scale
                );


                FLAlertLayer::create(
                    "AI LEVEL GENERATOR",
                    "Cancion agregada correctamente.",
                    "OK"
                )->show();
            }
        );
    }


    // ========================================================
    // ANALIZAR
    // ========================================================

    void onAnalyze(CCObject*) {

        if (!m_hasMusic) {

            FLAlertLayer::create(
                "AI LEVEL GENERATOR",
                "Primero agrega una musica.",
                "OK"
            )->show();

            return;
        }


        if (m_isAnalyzing) {

            FLAlertLayer::create(
                "AI LEVEL GENERATOR",
                "El analisis ya esta en progreso.",
                "OK"
            )->show();

            return;
        }


        m_isAnalyzing = true;

        m_hasAnalysis = false;

        m_analysisStatus->setString(
            "Decodificando MP3..."
        );


        auto musicPath =
            m_musicPath;


        geode::async::spawn(
            [musicPath]() -> arc::Future<std::pair<bool, AudioInfo>> {

                auto handle =
                    geode::async::runtime().spawnBlocking<
                        std::pair<bool, AudioInfo>
                    >(
                        [musicPath]() {

                            AudioInfo info;

                            bool success =
                                decodeMP3(
                                    musicPath,
                                    info
                                );

                            return std::make_pair(
                                success,
                                info
                            );
                        }
                    );

                co_return co_await handle;
            },

            [this](
                std::pair<bool, AudioInfo> result
            ) {

                m_isAnalyzing = false;


                if (!result.first) {

                    m_analysisStatus->setString(
                        "Error MP3"
                    );

                    FLAlertLayer::create(
                        "AI LEVEL GENERATOR",
                        "MPG123 no pudo decodificar el archivo MP3.",
                        "OK"
                    )->show();

                    return;
                }


                const auto& info =
                    result.second;


                m_hasAnalysis = true;


                char status[256];

                std::snprintf(
                    status,
                    sizeof(status),
                    "%.0f Hz  â€¢  %.1fs",
                    static_cast<double>(
                        info.sampleRate
                    ),
                    info.duration
                );


                m_analysisStatus->setString(
                    status
                );


                m_generationStatus->setString(
                    "PCM listo para analizar"
                );


                std::string message =
                    "MP3 decodificado correctamente.\n\n";

                message +=
                    "Sample rate: " +
                    std::to_string(
                        info.sampleRate
                    ) +
                    " Hz\n";

                message +=
                    "Canales: " +
                    std::to_string(
                        info.channels
                    ) +
                    "\n";

                message +=
                    "Duracion: ";


                char durationText[64];

                std::snprintf(
                    durationText,
                    sizeof(durationText),
                    "%.2f segundos",
                    info.duration
                );

                message +=
                    durationText;


                FLAlertLayer::create(
                    "ANALISIS MPG123",
                    message.c_str(),
                    "OK"
                )->show();
            }
        );
    }


    // ========================================================
    // GENERAR
    // ========================================================

    void onGenerate(CCObject*) {

        if (!m_hasMusic) {

            FLAlertLayer::create(
                "AI LEVEL GENERATOR",
                "Primero agrega una musica.",
                "OK"
            )->show();

            return;
        }


        if (!m_hasAnalysis) {

            FLAlertLayer::create(
                "AI LEVEL GENERATOR",
                "Primero analiza la musica.",
                "OK"
            )->show();

            return;
        }


        FLAlertLayer::create(
            "AI LEVEL GENERATOR",
            "El audio ya esta listo para la siguiente etapa del analizador.",
            "OK"
        )->show();
    }


    // ========================================================
    // CERRAR
    // ========================================================

    void onClose(CCObject*) {

        this->removeFromParentAndCleanup(
            true
        );
    }


public:

    static AIGeneratorPanel* create() {

        auto ret =
            new AIGeneratorPanel();

        if (
            ret &&
            ret->init()
        ) {

            ret->autorelease();

            return ret;
        }

        CC_SAFE_DELETE(ret);

        return nullptr;
    }
};


// ============================================================
// EDITOR UI
// ============================================================

class $modify(
    AILGEditorUI,
    EditorUI
) {

    struct Fields {

        CCMenuItemSpriteExtra*
            aiButton = nullptr;
    };


    bool init(
        LevelEditorLayer* editorLayer
    ) {

        if (!EditorUI::init(
            editorLayer
        ))
            return false;


        auto sprite =
            ButtonSprite::create(
                "AI",
                65,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                30.f,
                1.f
            );


        m_fields->aiButton =
            CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(
                    AILGEditorUI::openAI
                )
            );


        m_fields->aiButton->setPosition(
            185.f,
            75.f
        );


        this->addChild(
            m_fields->aiButton,
            100
        );


        return true;
    }


    void openAI(
        CCObject*
    ) {

        auto panel =
            AIGeneratorPanel::create();

        if (!panel)
            return;


        this->getParent()
            ->addChild(
                panel,
                1000
            );
    }
};
