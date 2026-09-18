#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;


// ============================================================
// PANEL DEL AI LEVEL GENERATOR
// ============================================================

class AIGeneratorPanel : public FLAlertLayer {
protected:

    CCLabelBMFont* m_musicLabel = nullptr;
    CCLabelBMFont* m_statusLabel = nullptr;
    CCLabelBMFont* m_analysisLabel = nullptr;

    bool m_hasMusic = false;


    CCLabelBMFont* createLabel(
        const char* text,
        const char* font,
        float scale,
        float x,
        float y
    ) {
        auto label = CCLabelBMFont::create(text, font);

        if (!label)
            return nullptr;

        label->setScale(scale);
        label->setPosition({x, y});

        this->addChild(label);

        return label;
    }


    CCMenuItemSpriteExtra* createButton(
        const char* text,
        SEL_MenuHandler callback,
        float x,
        float y,
        float width = 120.f
    ) {

        auto sprite = ButtonSprite::create(
            text,
            width,
            true,
            "goldFont.fnt",
            "GJ_button_01.png",
            30.f,
            1.f
        );

        if (!sprite)
            return nullptr;

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            callback
        );

        if (!button)
            return nullptr;

        button->setPosition({x, y});

        return button;
    }


    bool init() {

        // Ventana pequeña y controlada
        if (!FLAlertLayer::init(
            nullptr,
            "AI LEVEL GENERATOR",
            " ",
            "CERRAR",
            nullptr,
            420.f,
            false,
            260.f,
            0.9f
        )) {
            return false;
        }


        auto size = this->getContentSize();

        float cx = size.width / 2.f;


        // ====================================================
        // SUBTITULO
        // ====================================================

        createLabel(
            "MUSIC  ->  ANALYSIS  ->  GENERATION",
            "chatFont.fnt",
            0.34f,
            cx,
            205.f
        );


        // ====================================================
        // MUSICA
        // ====================================================

        createLabel(
            "MUSICA",
            "goldFont.fnt",
            0.40f,
            75.f,
            165.f
        );


        m_musicLabel = createLabel(
            "Ninguna seleccionada",
            "chatFont.fnt",
            0.42f,
            210.f,
            165.f
        );


        m_statusLabel = createLabel(
            "LISTO",
            "chatFont.fnt",
            0.34f,
            75.f,
            140.f
        );


        // ====================================================
        // BOTONES
        // ====================================================

        auto menu = CCMenu::create();

        if (!menu)
            return false;

        menu->setPosition({0.f, 0.f});


        auto addMusic = createButton(
            "AGREGAR",
            menu_selector(
                AIGeneratorPanel::onAddMusic
            ),
            335.f,
            165.f,
            100.f
        );


        auto analyze = createButton(
            "ANALIZAR",
            menu_selector(
                AIGeneratorPanel::onAnalyze
            ),
            130.f,
            90.f,
            110.f
        );


        auto generate = createButton(
            "GENERAR",
            menu_selector(
                AIGeneratorPanel::onGenerate
            ),
            290.f,
            90.f,
            110.f
        );


        if (addMusic)
            menu->addChild(addMusic);

        if (analyze)
            menu->addChild(analyze);

        if (generate) {
            generate->setEnabled(false);
            generate->setOpacity(120);
            menu->addChild(generate);
        }


        this->addChild(menu, 10);


        // ====================================================
        // ANALISIS
        // ====================================================

        createLabel(
            "ANALISIS",
            "goldFont.fnt",
            0.36f,
            75.f,
            55.f
        );


        m_analysisLabel = createLabel(
            "BPM   BEATS   ENERGIA",
            "chatFont.fnt",
            0.34f,
            250.f,
            55.f
        );


        return true;
    }


    // ========================================================
    // AGREGAR MUSICA
    // ========================================================

    void onAddMusic(CCObject*) {

        m_hasMusic = true;


        if (m_musicLabel) {

            m_musicLabel->setString(
                "Musica de prueba"
            );
        }


        if (m_statusLabel) {

            m_statusLabel->setString(
                "MUSICA LISTA"
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


        if (m_statusLabel) {

            m_statusLabel->setString(
                "ANALIZANDO..."
            );
        }


        if (m_analysisLabel) {

            m_analysisLabel->setString(
                "BPM   BEATS   ENERGIA   DROPS"
            );
        }


        FLAlertLayer::create(
            "ANALISIS IA",
            "Analisis preparado.\n\n"
            "La siguiente fase conectara "
            "el analizador real de audio.",
            "OK"
        )->show();
    }


    // ========================================================
    // GENERAR
    // ========================================================

    void onGenerate(CCObject*) {

        FLAlertLayer::create(
            "GENERADOR",
            "La generacion automatica del nivel "
            "se conectara despues del analisis "
            "real de la musica.",
            "OK"
        )->show();
    }


public:

    static AIGeneratorPanel* create() {

        auto ret = new AIGeneratorPanel();

        if (ret && ret->init()) {

            ret->autorelease();

            return ret;
        }

        CC_SAFE_DELETE(ret);

        return nullptr;
    }
};



// ============================================================
// EDITOR DE NIVELES
// ============================================================

class $modify(AILGEditorUI, EditorUI) {

    struct Fields {

        CCMenuItemSpriteExtra* aiButton = nullptr;

    };


    bool init(LevelEditorLayer* editorLayer) {

        if (!EditorUI::init(editorLayer))
            return false;


        auto size =
            CCDirector::sharedDirector()->getWinSize();


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


        // Tamaño pequeño para que no tape el editor
        button->setScale(0.72f);


        // ====================================================
        // POSICION DEL BOTON
        //
        // Se coloca en la zona izquierda del editor,
        // debajo de los controles superiores.
        // ====================================================

        auto menu = CCMenu::create();

        if (!menu)
            return true;


        menu->setPosition({
            190.f,
            75.f
        });


        menu->addChild(button);


        this->addChild(
            menu,
            100
        );


        m_fields->aiButton = button;


        return true;
    }


    // ========================================================
    // ABRIR AI
    // ========================================================

    void openAI(CCObject*) {

        auto panel =
            AIGeneratorPanel::create();


        if (panel) {

            panel->show();
        }
    }
};
