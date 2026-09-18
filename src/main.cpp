#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;


// ============================================================
// AI LEVEL GENERATOR
// PANEL DEL EDITOR
// ============================================================

class AIGeneratorPanel : public FLAlertLayer {
protected:

    CCLabelBMFont* m_musicName = nullptr;
    CCLabelBMFont* m_analysisStatus = nullptr;
    CCLabelBMFont* m_generationStatus = nullptr;

    bool m_hasMusic = false;
    bool m_hasAnalysis = false;


    // --------------------------------------------------------
    // CREAR TEXTO
    // --------------------------------------------------------

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
            10
        );

        return label;
    }


    // --------------------------------------------------------
    // CREAR FONDO DE TARJETA
    // --------------------------------------------------------

    CCLayerColor* makeCard(
        float x,
        float y,
        float width,
        float height
    ) {

        auto card = CCLayerColor::create(
            ccc4(
                5,
                35,
                65,
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
            0
        );

        return card;
    }


    // --------------------------------------------------------
    // BOTON
    // --------------------------------------------------------

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
    // INIT
    // ========================================================

    bool init() {

        if (!FLAlertLayer::init(
            nullptr,
            "AI LEVEL GENERATOR",
            " ",
            "CERRAR",
            nullptr,
            520.f,
            false,
            330.f,
            1.f
        )) {
            return false;
        }


        // ====================================================
        // IMPORTANTE
        //
        // Todas las coordenadas son del PANEL.
        // No usamos el tamaño de la pantalla.
        // ====================================================

        const float W = 520.f;
        const float H = 330.f;

        const float CX = W / 2.f;


        // ====================================================
        // ICONO AI
        // ====================================================

        auto icon =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );

        if (icon) {

            icon->setPosition({
                55.f,
                286.f
            });

            icon->setScale(
                0.65f
            );

            this->addChild(
                icon,
                10
            );
        }


        // ====================================================
        // SUBTITULO
        // ====================================================

        makeLabel(
            "CREA NIVELES CON INTELIGENCIA ARTIFICIAL",
            "goldFont.fnt",
            0.30f,
            290.f,
            285.f
        );


        // ====================================================
        // TARJETA PRINCIPAL
        // ====================================================

        makeCard(
            25.f,
            90.f,
            470.f,
            175.f
        );


        // ====================================================
        // SECCION MUSICA
        // ====================================================

        makeLabel(
            "MUSICA",
            "goldFont.fnt",
            0.38f,
            90.f,
            238.f
        );


        m_musicName = makeLabel(
            "Ninguna seleccionada",
            "chatFont.fnt",
            0.38f,
            155.f,
            214.f
        );


        // ====================================================
        // SECCION ANALISIS
        // ====================================================

        makeLabel(
            "ANALISIS",
            "goldFont.fnt",
            0.38f,
            90.f,
            180.f
        );


        m_analysisStatus = makeLabel(
            "Sin analisis",
            "chatFont.fnt",
            0.38f,
            155.f,
            157.f
        );


        // ====================================================
        // SECCION GENERACION
        // ====================================================

        makeLabel(
            "GENERACION",
            "goldFont.fnt",
            0.38f,
            100.f,
            122.f
        );


        m_generationStatus = makeLabel(
            "Sin generar",
            "chatFont.fnt",
            0.38f,
            155.f,
            100.f
        );


        // ====================================================
        // MENU
        // ====================================================

        auto menu = CCMenu::create();

        if (!menu)
            return false;

        menu->setPosition({
            0.f,
            0.f
        });


        // ----------------------------------------------------
        // AGREGAR
        // ----------------------------------------------------

        auto addButton = makeButton(
            "AGREGAR",
            menu_selector(
                AIGeneratorPanel::onAddMusic
            ),
            405.f,
            214.f,
            105.f
        );


        // ----------------------------------------------------
        // ANALIZAR
        // ----------------------------------------------------

        auto analyzeButton = makeButton(
            "ANALIZAR",
            menu_selector(
                AIGeneratorPanel::onAnalyze
            ),
            390.f,
            160.f,
            120.f
        );


        // ----------------------------------------------------
        // GENERAR
        // ----------------------------------------------------

        auto generateButton = makeButton(
            "GENERAR",
            menu_selector(
                AIGeneratorPanel::onGenerate
            ),
            390.f,
            105.f,
            120.f
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


        this->addChild(
            menu,
            20
        );


        // ====================================================
        // LINEA INFERIOR
        // ====================================================

        makeLabel(
            "AI ENGINE  •  EXPERIMENTAL",
            "chatFont.fnt",
            0.27f,
            CX,
            67.f
        );


        return true;
    }


    // ========================================================
    // AGREGAR MUSICA
    // ========================================================

    void onAddMusic(CCObject*) {

        m_hasMusic = true;


        if (m_musicName) {

            m_musicName->setString(
                "Musica seleccionada"
            );
        }


        FLAlertLayer::create(
            "MUSICA",
            "Musica de prueba seleccionada.\n\n"
            "El selector real de archivos de Android "
            "se conectara en la siguiente fase.",
            "OK"
        )->show();
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
                "BPM  •  BEATS  •  ENERGIA"
            );
        }


        if (m_generationStatus) {

            m_generationStatus->setString(
                "Listo para generar"
            );
        }


        FLAlertLayer::create(
            "ANALISIS IA",
            "Analisis de musica preparado.\n\n"
            "BPM, beats, energia y secciones "
            "seran utilizados por el generador.",
            "OK"
        )->show();
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
    // INIT DEL EDITOR
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
        // BOTON AI
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
        // MENU DEL BOTON
        // ====================================================

        auto menu =
            CCMenu::create();

        if (!menu)
            return true;


        // Posicion del boton AI
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

    void openAI(CCObject*) {

        auto panel =
            AIGeneratorPanel::create();

        if (panel) {

            panel->show();
        }
    }
};
