#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;


// ============================================================
// AI LEVEL GENERATOR - PANEL PROPIO
// ============================================================

class AIGeneratorPanel : public CCLayer {
protected:

    CCLabelBMFont* m_musicName = nullptr;
    CCLabelBMFont* m_analysisStatus = nullptr;
    CCLabelBMFont* m_generationStatus = nullptr;

    bool m_hasMusic = false;
    bool m_hasAnalysis = false;


    // ========================================================
    // TEXTO
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
    // BOTON
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
    // TARJETA
    // ========================================================

    CCLayerColor* makeCard(
        float x,
        float y,
        float width,
        float height
    ) {

        auto card = CCLayerColor::create(
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


        // ====================================================
        // ESTE LAYER OCUPA TODA LA PANTALLA
        // ====================================================

        this->setContentSize(screen);


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
        // TAMAÑO DEL PANEL
        // ====================================================

        const float panelW = 620.f;
        const float panelH = 365.f;

        const float panelX =
            (screen.width - panelW) / 2.f;

        const float panelY =
            (screen.height - panelH) / 2.f;


        // ====================================================
        // BORDE EXTERIOR
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
                82.f
            });

            header->setPosition({
                panelX + 4.f,
                panelY + panelH - 86.f
            });

            this->addChild(
                header,
                3
            );
        }


        // ====================================================
        // ICONO AI
        // ====================================================

        auto icon =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );

        if (icon) {

            icon->setScale(
                0.75f
            );

            icon->setPosition({
                panelX + 52.f,
                panelY + panelH - 43.f
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
            0.58f,
            panelX + panelW / 2.f,
            panelY + panelH - 36.f
        );


        // ====================================================
        // SUBTITULO
        // ====================================================

        makeLabel(
            "CREA NIVELES CON INTELIGENCIA ARTIFICIAL",
            "goldFont.fnt",
            0.27f,
            panelX + panelW / 2.f,
            panelY + panelH - 65.f
        );


        // ====================================================
        // TARJETA DE INFORMACION
        // ====================================================

        makeCard(
            panelX + 25.f,
            panelY + 92.f,
            350.f,
            165.f
        );


        // ====================================================
        // MUSICA
        // ====================================================

        makeLabel(
            "MUSICA",
            "goldFont.fnt",
            0.38f,
            panelX + 85.f,
            panelY + 225.f
        );


        m_musicName =
            makeLabel(
                "Ninguna seleccionada",
                "chatFont.fnt",
                0.34f,
                panelX + 195.f,
                panelY + 225.f
            );


        // ====================================================
        // LINEA
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
                315.f,
                2.f
            });

            line1->setPosition({
                panelX + 42.f,
                panelY + 195.f
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
            0.38f,
            panelX + 90.f,
            panelY + 165.f
        );


        m_analysisStatus =
            makeLabel(
                "Sin analisis",
                "chatFont.fnt",
                0.34f,
                panelX + 195.f,
                panelY + 165.f
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
                315.f,
                2.f
            });

            line2->setPosition({
                panelX + 42.f,
                panelY + 135.f
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
            0.38f,
            panelX + 100.f,
            panelY + 108.f
        );


        m_generationStatus =
            makeLabel(
                "Sin generar",
                "chatFont.fnt",
                0.34f,
                panelX + 205.f,
                panelY + 108.f
            );


        // ====================================================
        // MENU DE BOTONES
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
                panelX + 500.f,
                panelY + 220.f,
                105.f
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
                panelX + 495.f,
                panelY + 160.f,
                115.f
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
                panelX + 495.f,
                panelY + 100.f,
                115.f
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
            30
        );


        // ====================================================
        // BOTON CERRAR
        // ====================================================

        auto closeButton =
            makeButton(
                "X",
                menu_selector(
                    AIGeneratorPanel::onClose
                ),
                panelX + panelW - 25.f,
                panelY + panelH - 20.f,
                55.f
            );


        if (closeButton)
            menu->addChild(
                closeButton
            );


        // ====================================================
        // FOOTER
        // ====================================================

        makeLabel(
            "AI ENGINE  •  EXPERIMENTAL",
            "chatFont.fnt",
            0.26f,
            panelX + panelW / 2.f,
            panelY + 25.f
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
    // ABRIR AI
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
