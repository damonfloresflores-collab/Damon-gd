#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class AILGMainLayer : public FLAlertLayer {
protected:
    CCLabelBMFont* m_songLabel = nullptr;
    CCLabelBMFont* m_statusLabel = nullptr;
    CCLabelBMFont* m_analysisLabel = nullptr;

    bool m_hasMusic = false;

    bool init() {
        if (!FLAlertLayer::init(
            nullptr,
            "AI LEVEL GENERATOR",
            "Create levels from music",
            "CERRAR",
            nullptr,
            420.f,
            false,
            260.f,
            1.f
        )) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto title = CCLabelBMFont::create(
            "AI LEVEL GENERATOR",
            "bigFont.fnt"
        );
        title->setPosition({
            winSize.width / 2.f,
            winSize.height - 55.f
        });
        title->setScale(0.55f);
        this->addChild(title);

        auto subtitle = CCLabelBMFont::create(
            "Create levels from music",
            "goldFont.fnt"
        );
        subtitle->setPosition({
            winSize.width / 2.f,
            winSize.height - 82.f
        });
        subtitle->setScale(0.45f);
        this->addChild(subtitle);

        auto musicTitle = CCLabelBMFont::create(
            "MUSICA",
            "bigFont.fnt"
        );
        musicTitle->setPosition({
            winSize.width / 2.f,
            winSize.height - 120.f
        });
        musicTitle->setScale(0.42f);
        this->addChild(musicTitle);

        m_songLabel = CCLabelBMFont::create(
            "No hay musica seleccionada",
            "chatFont.fnt"
        );
        m_songLabel->setPosition({
            winSize.width / 2.f,
            winSize.height - 145.f
        });
        m_songLabel->setScale(0.55f);
        this->addChild(m_songLabel);

        auto addBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(
                "AGREGAR MUSICA",
                120,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                30.f,
                1.f
            ),
            this,
            menu_selector(AILGMainLayer::onAddMusic)
        );

        addBtn->setPosition({
            winSize.width / 2.f,
            winSize.height - 180.f
        });

        auto analyzeBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(
                "ANALIZAR MUSICA",
                120,
                true,
                "goldFont.fnt",
                "GJ_button_01.png",
                30.f,
                1.f
            ),
            this,
            menu_selector(AILGMainLayer::onAnalyze)
        );

        analyzeBtn->setPosition({
            winSize.width / 2.f,
            45.f
        });

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        menu->addChild(addBtn);
        menu->addChild(analyzeBtn);
        this->addChild(menu);

        m_statusLabel = CCLabelBMFont::create(
            "Estado: listo",
            "chatFont.fnt"
        );
        m_statusLabel->setPosition({
            winSize.width / 2.f,
            78.f
        });
        m_statusLabel->setScale(0.5f);
        this->addChild(m_statusLabel);

        m_analysisLabel = CCLabelBMFont::create(
            "Configuracion: Damon | IA decide | Sincronizacion extrema",
            "chatFont.fnt"
        );
        m_analysisLabel->setPosition({
            winSize.width / 2.f,
            105.f
        });
        m_analysisLabel->setScale(0.42f);
        this->addChild(m_analysisLabel);

        return true;
    }

    void onAddMusic(CCObject*) {
        m_hasMusic = true;

        m_songLabel->setString("Musica seleccionada");

        m_statusLabel->setString(
            "Estado: musica lista"
        );

        FLAlertLayer::create(
            "MUSICA",
            "La musica ha sido seleccionada.\n"
            "El selector real de Android se conectara en la siguiente fase.",
            "OK"
        )->show();
    }

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
            "Estado: analizando..."
        );

        m_analysisLabel->setString(
            "BPM | beats | energia | secciones | drops | instrumentos"
        );

        FLAlertLayer::create(
            "ANALISIS",
            "Modulo de analisis preparado.\n"
            "En la siguiente fase conectaremos el analizador "
            "real de audio y el mapa musical.",
            "CONTINUAR"
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

class $modify(AILGMenuLayer, MenuLayer) {
    struct Fields {};

    bool init() {
        if (!MenuLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            ),
            this,
            menu_selector(AILGMenuLayer::openGenerator)
        );

        btn->setScale(0.8f);

        auto menu = CCMenu::create();
        menu->setPosition({
            winSize.width - 35.f,
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
