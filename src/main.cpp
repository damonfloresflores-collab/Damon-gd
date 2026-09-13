#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class AILGMainLayer : public FLAlertLayer {
protected:
    CCLabelBMFont* m_songLabel = nullptr;
    CCLabelBMFont* m_statusLabel = nullptr;
    CCLabelBMFont* m_analysisLabel = nullptr;

    bool m_hasMusic = false;

    // Crear texto
    CCLabelBMFont* label(
        const char* text,
        const char* font,
        float scale,
        float x,
        float y
    ) {
        auto l = CCLabelBMFont::create(text, font);

        if (!l)
            return nullptr;

        l->setScale(scale);
        l->setPosition({x, y});
        this->addChild(l);

        return l;
    }

    // Crear tarjeta
    CCLayerColor* card(
        float x,
        float y,
        float w,
        float h
    ) {
        auto c = CCLayerColor::create(
            ccc4(10, 24, 42, 235)
        );

        if (!c)
            return nullptr;

        c->setContentSize({w, h});
        c->setPosition({x, y});

        this->addChild(c, -1);

        return c;
    }

    // Crear botón
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

        if (!sprite)
            return nullptr;

        auto item = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            callback
        );

        if (!item)
            return nullptr;

        item->setPosition({x, y});

        return item;
    }

    bool init() {

        // Ventana principal
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

        // IMPORTANTE:
        // Usamos el tamaño de la ventana del FLAlertLayer,
        // no el tamaño de toda la pantalla.
        auto size = this->getContentSize();

        float cx = size.width / 2.f;

        // ==========================================
        // SUBTITULO
        // ==========================================

        label(
            "MUSIC  ->  ANALYSIS  ->  GENERATION",
            "goldFont.fnt",
            0.34f,
            cx,
            276.f
        );

        // ==========================================
        // TARJETA DE MUSICA
        // ==========================================

        card(
            35.f,
            175.f,
            450.f,
            85.f
        );

        label(
            "MUSICA",
            "goldFont.fnt",
            0.38f,
            105.f,
            240.f
        );

        m_songLabel = label(
            "Ninguna musica seleccionada",
            "chatFont.fnt",
            0.45f,
            245.f,
            222.f
        );

        m_statusLabel = label(
            "LISTO",
            "chatFont.fnt",
            0.40f,
            100.f,
            194.f
        );

        // ==========================================
        // MENU DE BOTONES
        // ==========================================

        auto menu = CCMenu::create();

        if (!menu)
            return false;

        menu->setPosition({0.f, 0.f});

        // Boton agregar musica
        auto add = button(
            "AGREGAR MUSICA",
            menu_selector(AILGMainLayer::onAddMusic),
            390.f,
            220.f,
            125.f
        );

        // Boton analizar
        auto analyze = button(
            "ANALIZAR",
            menu_selector(AILGMainLayer::onAnalyze),
            170.f,
            85.f,
            125.f
        );

        // Boton generar
        auto generate = button(
            "GENERAR NIVEL",
            menu_selector(AILGMainLayer::onGenerate),
            350.f,
            85.f,
            135.f
        );

        if (add)
            menu->addChild(add);

        if (analyze)
            menu->addChild(analyze);

        if (generate) {
            generate->setEnabled(false);
            generate->setOpacity(120);
            menu->addChild(generate);
        }

        this->addChild(menu, 5);

        // ==========================================
        // TARJETA ANALISIS IA
        // ==========================================

        card(
            35.f,
            105.f,
            450.f,
            55.f
        );

        label(
            "ANALISIS IA",
            "goldFont.fnt",
            0.36f,
            100.f,
            145.f
        );

        m_analysisLabel = label(
            "BPM   BEATS   ENERGIA   SECCIONES",
            "chatFont.fnt",
            0.38f,
            300.f,
            145.f
        );

        // ==========================================
        // DESCRIPCION
        // ==========================================

        label(
            "La IA decidira la sincronizacion del gameplay.",
            "chatFont.fnt",
            0.34f,
            cx,
            118.f
        );

        // ==========================================
        // FOOTER
        // ==========================================

        label(
            "AI ENGINE  •  EXPERIMENTAL",
            "chatFont.fnt",
            0.28f,
            cx,
            67.f
        );

        return true;
    }

    // ==========================================
    // AGREGAR MUSICA
    // ==========================================

    void onAddMusic(CCObject*) {

        m_hasMusic = true;

        if (m_songLabel) {
            m_songLabel->setString(
                "Musica de prueba seleccionada"
            );
        }

        if (m_statusLabel) {
            m_statusLabel->setString(
                "MUSICA LISTA"
            );
        }

        FLAlertLayer::create(
            "MUSICA",
            "Musica de prueba seleccionada.\n"
            "El selector real de archivos de Android "
            "sera conectado en la siguiente fase.",
            "OK"
        )->show();
    }

    // ==========================================
    // ANALIZAR
    // ==========================================

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
                "ANALIZANDO"
            );
        }

        if (m_analysisLabel) {
            m_analysisLabel->setString(
                "BPM | BEATS | ENERGIA | DROPS | SECCIONES"
            );
        }

        FLAlertLayer::create(
            "ANALISIS IA",
            "Interfaz de analisis lista.\n"
            "La proxima fase conectara el analizador "
            "real de audio.",
            "CONTINUAR"
        )->show();
    }

    // ==========================================
    // GENERAR NIVEL
    // ==========================================

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


// ==============================================
// BOTON DEL MOD EN EL MENU PRINCIPAL DE GD
// ==============================================

class $modify(AILGMenuLayer, MenuLayer) {

    struct Fields {};

    bool init() {

        if (!MenuLayer::init())
            return false;

        auto size =
            CCDirector::sharedDirector()->getWinSize();

        // Icono del mod
        auto sprite =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );

        if (!sprite)
            return true;

        auto btn =
            CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(
                    AILGMenuLayer::openGenerator
                )
            );

        if (!btn)
            return true;

        btn->setScale(0.8f);

        // Menu del boton
        auto menu = CCMenu::create();

        if (!menu)
            return true;

        menu->setPosition({
            size.width - 35.f,
            35.f
        });

        menu->addChild(btn);

        this->addChild(menu, 10);

        return true;
    }

    // Abrir generador
    void openGenerator(CCObject*) {

        auto generator = AILGMainLayer::create();

        if (generator) {
            generator->show();
        }
    }
};
