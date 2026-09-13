#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class AILGMainLayer : public FLAlertLayer {
protected:
    CCLabelBMFont* m_songLabel = nullptr;
    CCLabelBMFont* m_statusLabel = nullptr;
    CCLabelBMFont* m_analysisLabel = nullptr;

    bool m_hasMusic = false;

    CCLabelBMFont* label(
        const char* text,
        const char* font,
        float scale,
        float x,
        float y
    ) {
        auto l = CCLabelBMFont::create(text, font);
        l->setScale(scale);
        l->setPosition({x, y});
        this->addChild(l);
        return l;
    }

    CCLayerColor* card(
        float x,
        float y,
        float w,
        float h
    ) {
        auto c = CCLayerColor::create(
            ccc4(10, 24, 42, 235),
            {w, h}
        );

        c->setPosition({x, y});
        this->addChild(c);

        return c;
    }

    CCMenuItemSpriteExtra* button(
        const char* text,
        SEL_MenuHandler callback,
        float x,
        float y,
        float width = 135.f
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

        auto item = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            callback
        );

        item->setPosition({x, y});

        return item;
    }

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

        auto size = CCDirector::sharedDirector()->getWinSize();
        auto cx = size.width / 2.f;


        // =========================
        // HEADER
        // =========================

        label(
            "AI LEVEL GENERATOR",
            "bigFont.fnt",
            0.58f,
            cx,
            size.height - 63.f
        );

        label(
            "MUSIC  ->  ANALYSIS  ->  GENERATION",
            "goldFont.fnt",
            0.34f,
            cx,
            size.height - 91.f
        );


        // =========================
        // MUSIC CARD
        // =========================

        card(
            cx - 225.f,
            size.height - 220.f,
            450.f,
            92.f
        );

        label(
            "MUSICA",
            "goldFont.fnt",
            0.38f,
            cx - 175.f,
            size.height - 153.f
        );

        m_songLabel = label(
            "Ninguna musica seleccionada",
            "chatFont.fnt",
            0.52f,
            cx - 75.f,
            size.height - 187.f
        );

        m_statusLabel = label(
            "LISTO",
            "chatFont.fnt",
            0.42f,
            cx - 170.f,
            size.height - 207.f
        );


        // =========================
        // BOTONES
        // =========================

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});


        auto add = button(
            "AGREGAR MUSICA",
            menu_selector(AILGMainLayer::onAddMusic),
            cx + 125.f,
            size.height - 187.f,
            125.f
        );


        auto analyze = button(
            "ANALIZAR",
            menu_selector(AILGMainLayer::onAnalyze),
            cx - 145.f,
            78.f,
            125.f
        );


        auto generate = button(
            "GENERAR NIVEL",
            menu_selector(AILGMainLayer::onGenerate),
            cx + 145.f,
            78.f,
            125.f
        );

        generate->setEnabled(false);
        generate->setOpacity(120);


        menu->addChild(add);
        menu->addChild(analyze);
        menu->addChild(generate);

        this->addChild(menu, 5);


        // =========================
        // ANALISIS IA
        // =========================

        card(
            cx - 225.f,
            120.f,
            450.f,
            105.f
        );

        label(
            "ANALISIS IA",
            "goldFont.fnt",
            0.38f,
            cx,
            193.f
        );


        m_analysisLabel = label(
            "BPM      BEATS      ENERGIA      SECCIONES",
            "chatFont.fnt",
            0.40f,
            cx,
            163.f
        );


        label(
            "La IA decidira la sincronizacion del gameplay.",
            "chatFont.fnt",
            0.36f,
            cx,
            138.f
        );


        // =========================
        // FOOTER
        // =========================

        label(
            "AI ENGINE  •  EXPERIMENTAL",
            "chatFont.fnt",
            0.30f,
            cx,
            51.f
        );


        return true;
    }


    // =========================
    // AGREGAR MUSICA
    // =========================

    void onAddMusic(CCObject*) {

        m_hasMusic = true;

        m_songLabel->setString(
            "Musica de prueba seleccionada"
        );

        m_statusLabel->setString(
            "MUSICA LISTA"
        );


        FLAlertLayer::create(
            "MUSICA",
            "Musica de prueba seleccionada.\n"
            "El selector real de archivos de Android "
            "sera conectado en la siguiente fase.",
            "OK"
        )->show();
    }


    // =========================
    // ANALIZAR
    // =========================

    void onAnalyze(CCObject*) {

        if (!m_hasMusic) {

            FLAlertLayer::create(
                "AI LEVEL GENERATOR",
                "Primero agrega una musica.",
                "OK"
            )->show();

            return;
        }


        m_statusLabel->setString(
            "ANALIZANDO"
        );


        m_analysisLabel->setString(
            "BPM | BEATS | ENERGIA | DROPS | SECCIONES"
        );


        FLAlertLayer::create(
            "ANALISIS IA",
            "Interfaz de analisis lista.\n"
            "La proxima fase conectara el analizador "
            "real de audio.",
            "CONTINUAR"
        )->show();
    }


    // =========================
    // GENERAR NIVEL
    // =========================

    void onGenerate(CCObject*) {

        FLAlertLayer::create(
            "GENERADOR",
            "La generacion del nivel se habilitara "
            "despues de conectar\n"
            "el analisis real de la musica.",
            "OK"
        )->show();
    }


public:

    static AILGMainLayer* create() {

        auto ret = new AILGMainLayer();

        if (ret && ret->init()) {

            ret->autorelease();

            return ret;
        }

        CC_SAFE_DELETE(ret);

        return nullptr;
    }
};


// =========================
// BOTON EN MENU PRINCIPAL
// =========================

class $modify(AILGMenuLayer, MenuLayer) {

    struct Fields {};


    bool init() {

        if (!MenuLayer::init())
            return false;


        auto size =
            CCDirector::sharedDirector()->getWinSize();


        auto sprite =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );


        auto btn =
            CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(
                    AILGMenuLayer::openGenerator
                )
            );


        btn->setScale(0.8f);


        auto menu = CCMenu::create();

        menu->setPosition({
            size.width - 35.f,
            35.f
        });


        menu->addChild(btn);

        this->addChild(menu, 10);


        return true;
    }


    void openGenerator(CCObject*) {

        AILGMainLayer::create()->show();
    }
};
