#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/file.hpp>

#include <filesystem>
#include <algorithm>
#include <cctype>
#include <string>

using namespace geode::prelude;


// ============================================================
// AI LEVEL GENERATOR
// ============================================================

class AIGeneratorPanel : public CCLayer {
protected:

    CCLabelBMFont* m_musicName = nullptr;
    CCLabelBMFont* m_analysisStatus = nullptr;
    CCLabelBMFont* m_generationStatus = nullptr;

    // Ruta del MP3 seleccionado
    std::filesystem::path m_musicPath;

    bool m_hasMusic = false;
    bool m_hasAnalysis = false;


    // ========================================================
    // CREAR LABEL
    // ========================================================

    CCLabelBMFont* makeLabel(
        const char* text,
        const char* font,
        float scale,
        float x,
        float y
    ) {

        auto label = CCLabelBMFont::create(
            text,
            font
        );

        if (!label)
            return nullptr;

        label->setScale(scale);

        label->setPosition({
            x,
            y
        });

        this->addChild(
            label,
            20
        );

        return label;
    }


    // ========================================================
    // CREAR BOTON
    // ========================================================

    CCMenuItemSpriteExtra* makeButton(
        const char* text,
        SEL_MenuHandler callback,
        float x,
        float y,
        float width
    ) {

        auto sprite = ButtonSprite::create(
            text,
            width,
            true,
            "goldFont.fnt",
            "GJ_button_01.png",
            28.f,
            1.f
        );

        if (!sprite)
            return nullptr;

        auto button =
            CCMenuItemSpriteExtra::create(
                sprite,
                this,
                callback
            );

        if (!button)
            return nullptr;

        button->setPosition({
            x,
            y
        });

        return button;
    }


    // ========================================================
    // CREAR TARJETA
    // ========================================================

    CCLayerColor* makeCard(
        float x,
        float y,
        float width,
        float height
    ) {

        auto card =
            CCLayerColor::create(
                ccc4(
                    5,
                    32,
                    58,
                    245
                )
            );

        if (!card)
            return nullptr;

        card->setContentSize({
            width,
            height
        });

        card->setPosition({
            x,
            y
        });

        this->addChild(
            card,
            5
        );

        return card;
    }


    // ========================================================
    // INIT
    // ========================================================

    bool init() {

        if (!CCLayer::init())
            return false;


        auto screen =
            CCDirector::sharedDirector()->getWinSize();


        this->setContentSize(
            screen
        );


        // ====================================================
        // FONDO OSCURO
        // ====================================================

        auto overlay =
            CCLayerColor::create(
                ccc4(
                    0,
                    0,
                    0,
                    150
                )
            );

        if (overlay) {

            overlay->setContentSize(
                screen
            );

            overlay->setPosition({
                0.f,
                0.f
            });

            this->addChild(
                overlay,
                -10
            );
        }


        // ====================================================
        // TAMAÑO DEL PANEL
        // ====================================================

        const float panelW = 540.f;
        const float panelH = 320.f;


        const float panelX =
            (screen.width - panelW) / 2.f;


        const float panelY =
            (screen.height - panelH) / 2.f;


        // ====================================================
        // BORDE CELESTE
        // ====================================================

        auto border =
            CCLayerColor::create(
                ccc4(
                    0,
                    190,
                    255,
                    255
                )
            );

        if (border) {

            border->setContentSize({
                panelW,
                panelH
            });

            border->setPosition({
                panelX,
                panelY
            });

            this->addChild(
                border,
                1
            );
        }


        // ====================================================
        // PANEL PRINCIPAL
        // ====================================================

        auto panel =
            CCLayerColor::create(
                ccc4(
                    5,
                    22,
                    42,
                    255
                )
            );

        if (panel) {

            panel->setContentSize({
                panelW - 8.f,
                panelH - 8.f
            });

            panel->setPosition({
                panelX + 4.f,
                panelY + 4.f
            });

            this->addChild(
                panel,
                2
            );
        }


        // ====================================================
        // CABECERA
        // ====================================================

        auto header =
            CCLayerColor::create(
                ccc4(
                    8,
                    45,
                    75,
                    255
                )
            );

        if (header) {

            header->setContentSize({
                panelW - 8.f,
                70.f
            });

            header->setPosition({
                panelX + 4.f,
                panelY + panelH - 74.f
            });

            this->addChild(
                header,
                3
            );
        }


        // ====================================================
        // ICONO INFO
        // ====================================================

        auto icon =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );

        if (icon) {

            icon->setScale(
                0.65f
            );

            icon->setPosition({
                panelX + 42.f,
                panelY + panelH - 37.f
            });

            this->addChild(
                icon,
                25
            );
        }


        // ====================================================
        // TITULO
        // ====================================================

        makeLabel(
            "AI LEVEL GENERATOR",
            "bigFont.fnt",
            0.52f,
            panelX + panelW / 2.f,
            panelY + panelH - 31.f
        );


        // ====================================================
        // SUBTITULO
        // ====================================================

        makeLabel(
            "CREA NIVELES CON INTELIGENCIA ARTIFICIAL",
            "goldFont.fnt",
            0.24f,
            panelX + panelW / 2.f,
            panelY + panelH - 56.f
        );


        // ====================================================
        // TARJETA DE INFORMACION
        // ====================================================

        makeCard(
            panelX + 20.f,
            panelY + 72.f,
            305.f,
            145.f
        );


        // ====================================================
        // MUSICA
        // ====================================================

        makeLabel(
            "MUSICA",
            "goldFont.fnt",
            0.34f,
            panelX + 72.f,
            panelY + 196.f
        );


        m_musicName =
            makeLabel(
                "Ninguna seleccionada",
                "chatFont.fnt",
                0.29f,
                panelX + 175.f,
                panelY + 196.f
            );


        // ====================================================
        // LINEA 1
        // ====================================================

        auto line1 =
            CCLayerColor::create(
                ccc4(
                    30,
                    100,
                    145,
                    255
                )
            );

        if (line1) {

            line1->setContentSize({
                275.f,
                2.f
            });

            line1->setPosition({
                panelX + 35.f,
                panelY + 169.f
            });

            this->addChild(
                line1,
                10
            );
        }


        // ====================================================
        // ANALISIS
        // ====================================================

        makeLabel(
            "ANALISIS",
            "goldFont.fnt",
            0.34f,
            panelX + 76.f,
            panelY + 139.f
        );


        m_analysisStatus =
            makeLabel(
                "Sin analisis",
                "chatFont.fnt",
                0.29f,
                panelX + 175.f,
                panelY + 139.f
            );


        // ====================================================
        // LINEA 2
        // ====================================================

        auto line2 =
            CCLayerColor::create(
                ccc4(
                    30,
                    100,
                    145,
                    255
                )
            );

        if (line2) {

            line2->setContentSize({
                275.f,
                2.f
            });

            line2->setPosition({
                panelX + 35.f,
                panelY + 113.f
            });

            this->addChild(
                line2,
                10
            );
        }


        // ====================================================
        // GENERACION
        // ====================================================

        makeLabel(
            "GENERACION",
            "goldFont.fnt",
            0.34f,
            panelX + 82.f,
            panelY + 84.f
        );


        m_generationStatus =
            makeLabel(
                "Sin generar",
                "chatFont.fnt",
                0.29f,
                panelX + 180.f,
                panelY + 84.f
            );


        // ====================================================
        // MENU
        // ====================================================

        auto menu =
            CCMenu::create();

        if (!menu)
            return false;


        menu->setPosition({
            0.f,
            0.f
        });


        // ====================================================
        // AGREGAR
        // ====================================================

        auto addButton =
            makeButton(
                "AGREGAR",
                menu_selector(
                    AIGeneratorPanel::onAddMusic
                ),
                panelX + 435.f,
                panelY + 188.f,
                100.f
            );


        // ====================================================
        // ANALIZAR
        // ====================================================

        auto analyzeButton =
            makeButton(
                "ANALIZAR",
                menu_selector(
                    AIGeneratorPanel::onAnalyze
                ),
                panelX + 435.f,
                panelY + 130.f,
                105.f
            );


        // ====================================================
        // GENERAR
        // ====================================================

        auto generateButton =
            makeButton(
                "GENERAR",
                menu_selector(
                    AIGeneratorPanel::onGenerate
                ),
                panelX + 435.f,
                panelY + 72.f,
                105.f
            );


        if (addButton)
            menu->addChild(
                addButton
            );


        if (analyzeButton)
            menu->addChild(
                analyzeButton
            );


        if (generateButton) {

            generateButton->setEnabled(
                false
            );

            generateButton->setOpacity(
                120
            );

            menu->addChild(
                generateButton
            );
        }


        // ====================================================
        // CERRAR
        // ====================================================

        auto closeButton =
            makeButton(
                "X",
                menu_selector(
                    AIGeneratorPanel::onClose
                ),
                panelX + panelW - 22.f,
                panelY + panelH - 18.f,
                48.f
            );


        if (closeButton)
            menu->addChild(
                closeButton
            );


        this->addChild(
            menu,
            30
        );


        // ====================================================
        // FOOTER
        // ====================================================

        makeLabel(
            "AI ENGINE  •  EXPERIMENTAL",
            "chatFont.fnt",
            0.22f,
            panelX + panelW / 2.f,
            panelY + 19.f
        );


        return true;
    }


    // ========================================================
    // AGREGAR MUSICA
    // ========================================================

    void onAddMusic(CCObject*) {

        // ----------------------------------------------------
        // OPCIONES DEL SELECTOR
        // ----------------------------------------------------

        geode::utils::file::FilePickOptions options;


        // ----------------------------------------------------
        // FILTRO MP3
        // ----------------------------------------------------

        geode::utils::file::FilePickOptions::Filter mp3Filter;

        mp3Filter.description =
            "Audio MP3";

        mp3Filter.files.insert(
            ".mp3"
        );

        options.filters.push_back(
            mp3Filter
        );


        // ----------------------------------------------------
        // ABRIR SELECTOR DE ANDROID
        // ----------------------------------------------------

        geode::async::spawn(

            geode::utils::file::pick(
                geode::utils::file::PickMode::OpenFile,
                options
            ),

            [this](
                geode::utils::file::PickResult result
            ) {

                // ============================================
                // ERROR
                // ============================================

                if (!result.isOk()) {

                    FLAlertLayer::create(
                        "MUSICA",
                        "No se pudo abrir el selector de archivos.",
                        "OK"
                    )->show();

                    return;
                }


                // ============================================
                // ARCHIVO SELECCIONADO
                // ============================================

                auto selected =
                    result.unwrap();


                // ============================================
                // CANCELADO
                // ============================================

                if (!selected) {
                    return;
                }


                // ============================================
                // GUARDAR RUTA
                // ============================================

                m_musicPath =
                    selected.value();


                // ============================================
                // COMPROBAR EXTENSION
                // ============================================

                std::string extension =
                    m_musicPath.extension().string();


                std::transform(
                    extension.begin(),
                    extension.end(),
                    extension.begin(),
                    [](unsigned char c) {

                        return static_cast<char>(
                            std::tolower(c)
                        );
                    }
                );


                // ============================================
                // SOLO MP3
                // ============================================

                if (extension != ".mp3") {

                    m_musicPath.clear();

                    m_hasMusic = false;


                    if (m_musicName) {

                        m_musicName->setString(
                            "Solo archivos MP3"
                        );

                        m_musicName->setScale(
                            0.29f
                        );
                    }


                    FLAlertLayer::create(
                        "MUSICA",
                        "Selecciona una cancion en formato MP3.",
                        "OK"
                    )->show();

                    return;
                }


                // ============================================
                // OBTENER NOMBRE
                // ============================================

                std::string songName =
                    m_musicPath.stem().string();


                if (songName.empty()) {

                    songName =
                        m_musicPath.filename().string();
                }


                // ============================================
                // GUARDAR ESTADO
                // ============================================

                m_hasMusic = true;

                m_hasAnalysis = false;


                // ============================================
                // ACTUALIZAR ANALISIS
                // ============================================

                if (m_analysisStatus) {

                    m_analysisStatus->setString(
                        "Sin analisis"
                    );
                }


                if (m_generationStatus) {

                    m_generationStatus->setString(
                        "Sin generar"
                    );
                }


                // ============================================
                // MOSTRAR NOMBRE
                // ============================================

                if (m_musicName) {

                    m_musicName->setString(
                        songName.c_str()
                    );


                    // Ajustar tamaño para nombres largos
                    float scale = 0.29f;

                    if (songName.length() > 25)
                        scale = 0.25f;

                    if (songName.length() > 35)
                        scale = 0.22f;

                    if (songName.length() > 45)
                        scale = 0.19f;


                    m_musicName->setScale(
                        scale
                    );
                }


                // ============================================
                // CONFIRMACION
                // ============================================

                FLAlertLayer::create(
                    "MUSICA",
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


        m_hasAnalysis = true;


        if (m_analysisStatus) {

            m_analysisStatus->setString(
                "BPM  BEATS  ENERGIA"
            );
        }


        if (m_generationStatus) {

            m_generationStatus->setString(
                "Listo para generar"
            );
        }
    }


    // ========================================================
    // GENERAR
    // ========================================================

    void onGenerate(CCObject*) {

        if (!m_hasMusic) {

            FLAlertLayer::create(
                "GENERADOR",
                "Primero agrega una musica.",
                "OK"
            )->show();

            return;
        }


        if (!m_hasAnalysis) {

            FLAlertLayer::create(
                "GENERADOR",
                "Primero analiza la musica.",
                "OK"
            )->show();

            return;
        }


        if (m_generationStatus) {

            m_generationStatus->setString(
                "Generando..."
            );
        }


        FLAlertLayer::create(
            "AI LEVEL GENERATOR",
            "La generacion automatica del nivel "
            "se conectara en la siguiente fase.",
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

    // ========================================================
    // CREATE
    // ========================================================

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


        CC_SAFE_DELETE(
            ret
        );


        return nullptr;
    }
};


// ============================================================
// EDITOR UI
// ============================================================

class $modify(AILGEditorUI, EditorUI) {

    struct Fields {

        CCMenuItemSpriteExtra* aiButton =
            nullptr;
    };


    // ========================================================
    // INIT EDITOR
    // ========================================================

    bool init(
        LevelEditorLayer* editorLayer
    ) {

        if (!EditorUI::init(
            editorLayer
        )) {

            return false;
        }


        // ====================================================
        // SPRITE AI
        // ====================================================

        auto sprite =
            ButtonSprite::create(
                "AI",
                65.f,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                28.f,
                1.f
            );


        if (!sprite)
            return true;


        // ====================================================
        // BOTON AI
        // ====================================================

        auto button =
            CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(
                    AILGEditorUI::openAI
                )
            );


        if (!button)
            return true;


        button->setScale(
            0.75f
        );


        // ====================================================
        // MENU
        // ====================================================

        auto menu =
            CCMenu::create();


        if (!menu)
            return true;


        menu->setPosition({
            185.f,
            75.f
        });


        menu->addChild(
            button
        );


        this->addChild(
            menu,
            100
        );


        m_fields->aiButton =
            button;


        return true;
    }


    // ========================================================
    // ABRIR PANEL
    // ========================================================

    void openAI(
        CCObject*
    ) {

        auto panel =
            AIGeneratorPanel::create();


        if (panel) {

            this->addChild(
                panel,
                1000
            );
        }
    }
};
