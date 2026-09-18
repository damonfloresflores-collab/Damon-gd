#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;


// ============================================================
// AI LEVEL GENERATOR - PANEL
// ============================================================

class AIGeneratorPanel : public CCLayer {
protected:

    CCLabelBMFont* m_musicName = nullptr;
    CCLabelBMFont* m_analysisStatus = nullptr;
    CCLabelBMFont* m_generationStatus = nullptr;

    bool m_hasMusic = false;
    bool m_hasAnalysis = false;


    // ========================================================
    // LABEL
    // ========================================================

    CCLabelBMFont* makeLabel(
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

        this->addChild(label, 20);

        return label;
    }


    // ========================================================
    // BUTTON
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


    // ========================================================
    // CARD
    // ========================================================

    CCLayerColor* makeCard(
        float x,
        float y,
        float width,
        float height
    ) {

        auto card = CCLayerColor::create(
            ccc4(5, 32, 58, 245)
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

        this->addChild(card, 5);

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

        this->setContentSize(screen);


        // ====================================================
        // DARK OVERLAY
        // ====================================================

        auto overlay =
            CCLayerColor::create(
                ccc4(0, 0, 0, 150)
            );

        if (overlay) {

            overlay->setContentSize(screen);

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
        // PANEL SIZE
        // REDUCIDO
        // ====================================================

        const float panelW = 540.f;
        const float panelH = 320.f;

        const float panelX =
            (screen.width - panelW) / 2.f;

        const float panelY =
            (screen.height - panelH) / 2.f;


        // ====================================================
        // CYAN BORDER
        // ====================================================

        auto border =
            CCLayerColor::create(
                ccc4(0, 190, 255, 255)
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
        // MAIN PANEL
        // ====================================================

        auto panel =
            CCLayerColor::create(
                ccc4(5, 22, 42, 255)
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
        // HEADER
        // ====================================================

        auto header =
            CCLayerColor::create(
                ccc4(8, 45, 75, 255)
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
        // INFO ICON
        // ====================================================

        auto icon =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );

        if (icon) {

            icon->setScale(0.65f);

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
        // TITLE
        // ====================================================

        makeLabel(
            "AI LEVEL GENERATOR",
            "bigFont.fnt",
            0.52f,
            panelX + panelW / 2.f,
            panelY + panelH - 31.f
        );


        // ====================================================
        // SUBTITLE
        // ====================================================

        makeLabel(
            "CREA NIVELES CON INTELIGENCIA ARTIFICIAL",
            "goldFont.fnt",
            0.24f,
            panelX + panelW / 2.f,
            panelY + panelH - 56.f
        );


        // ====================================================
        // INFORMATION CARD
        // ====================================================

        makeCard(
            panelX + 20.f,
            panelY + 72.f,
            305.f,
            145.f
        );


        // ====================================================
        // MUSIC
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
        // LINE 1
        // ====================================================

        auto line1 =
            CCLayerColor::create(
                ccc4(30, 100, 145, 255)
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
        // ANALYSIS
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
        // LINE 2
        // ====================================================

        auto line2 =
            CCLayerColor::create(
                ccc4(30, 100, 145, 255)
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
        // GENERATION
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
        // BUTTON MENU
        // ====================================================

        auto menu = CCMenu::create();

        if (!menu)
            return false;

        menu->setPosition({
            0.f,
            0.f
        });


        // ====================================================
        // ADD
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
        // ANALYZE
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
        // GENERATE
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
            menu->addChild(addButton);

        if (analyzeButton)
            menu->addChild(analyzeButton);

        if (generateButton) {

            generateButton->setEnabled(false);

            generateButton->setOpacity(120);

            menu->addChild(
                generateButton
            );
        }


        // ====================================================
        // CLOSE
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
            menu->addChild(closeButton);


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
    // ADD MUSIC
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
    // ANALYZE
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
    // GENERATE
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
    // CLOSE
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

class $modify(AILGEditorUI, EditorUI) {

    struct Fields {

        CCMenuItemSpriteExtra* aiButton = nullptr;

    };


    bool init(
        LevelEditorLayer* editorLayer
    ) {

        if (!EditorUI::init(editorLayer))
            return false;


        // ====================================================
        // AI BUTTON
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


        button->setScale(0.75f);


        auto menu = CCMenu::create();

        if (!menu)
            return true;


        menu->setPosition({
            185.f,
            75.f
        });

        menu->addChild(button);


        this->addChild(
            menu,
            100
        );


        m_fields->aiButton =
            button;


        return true;
    }


    // ========================================================
    // OPEN AI
    // ========================================================

    void openAI(CCObject*) {

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
