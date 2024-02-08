#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <string>
#include <fmt/format.h>
#include <vector>

using namespace geode::prelude;

std::string getStatsIconSpriteString(int i);
void compareStats(int A, int B, std::string strA, std::string strB, bool isGreater);



struct statsInfo {
	int value;
	std::string str;
};

class CompareStatsPopup : public geode::Popup<std::string const&> {
    protected:
    bool setup(std::string const& value) override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setTitle("P adu stats");

        auto label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
        label->setPosition(winSize / 2);
        this->addChild(label);

        return true;
    }

public:
    static CompareStatsPopup* create(std::string const& text) {
        auto ret = new CompareStatsPopup();
        if (ret && ret->init(470.f, 290.f, text)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

};

class $modify(CompStatsProfilePage, ProfilePage) {
	GameManager* m_gameManager;

	void onCompareButton(CCObject* sender){
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto popup = CompareStatsPopup::create("");
		auto statsA = GameStatsManager::sharedState()->m_playerStats;
		auto statsB = static_cast<GJUserScore*>(static_cast<CCNode*>(sender)->getUserObject());
		
		#pragma region player container
		#pragma region init
		auto playerContainer = CCMenu::create();
		playerContainer->setLayout(
			RowLayout::create()->setGap(87.f)->setAxisAlignment(AxisAlignment::Center)
		);
		playerContainer->setPosition({winSize.width / 2, winSize.height / 2 + 86});
		playerContainer->setContentSize({430, 50});
		playerContainer->setID("player-container"_spr);
		#pragma endregion
		#pragma region playerA
		auto playerALabel = CCLabelBMFont::create(m_fields->m_gameManager->m_playerName.c_str(), "goldFont.fnt");
		playerALabel->setID("player-a-label"_spr);

		#pragma region cubeA
		auto cubeA = SimplePlayer::create(m_fields->m_gameManager->getPlayerFrame());
		cubeA->setColor(m_fields->m_gameManager->colorForIdx(m_fields->m_gameManager->getPlayerColor()));
		cubeA->setSecondColor(m_fields->m_gameManager->colorForIdx(m_fields->m_gameManager->getPlayerColor2()));
		cubeA->setGlowOutline(m_fields->m_gameManager->colorForIdx(m_fields->m_gameManager->getPlayerGlowColor()));
		cubeA->enableCustomGlowColor(m_fields->m_gameManager->colorForIdx(m_fields->m_gameManager->getPlayerGlowColor()));

		if(!m_fields->m_gameManager->m_playerGlow) cubeA->disableGlowOutline();
		#pragma endregion

		cubeA->setID("player-a-icon"_spr);
		
		auto playerAContainer = CCNode::create();
		playerAContainer->setLayout(
			RowLayout::create()->setGap(30.f)->setAxisAlignment(AxisAlignment::End)
		);
		playerAContainer->setContentSize({200, 50});
		playerAContainer->setID("player-a-container"_spr);
		playerAContainer->addChild(playerALabel);
		playerAContainer->addChild(cubeA);
		playerAContainer->updateLayout();

		playerContainer->addChild(playerAContainer);
		#pragma endregion
		#pragma region playerB

		auto playerBLabel = CCLabelBMFont::create(statsB->m_userName.c_str(), "goldFont.fnt");
		playerBLabel->setID("player-b-label"_spr);

		#pragma region cubeB
		auto cubeB = SimplePlayer::create(statsB->m_playerCube);
		cubeB->setColor(m_fields->m_gameManager->colorForIdx(statsB->m_color1));
		cubeB->setSecondColor(m_fields->m_gameManager->colorForIdx(statsB->m_color2));
		cubeB->setGlowOutline(m_fields->m_gameManager->colorForIdx(statsB->m_color3));
		cubeB->enableCustomGlowColor(m_fields->m_gameManager->colorForIdx(statsB->m_color3));

		if(!statsB->m_glowEnabled) cubeB->disableGlowOutline();
		#pragma endregion
		cubeB->setID("player-b-icon"_spr);

		auto playerBContainer = CCNode::create();
		playerBContainer->setLayout(
			RowLayout::create()->setGap(30.f)->setAxisAlignment(AxisAlignment::Start)
		);
		playerBContainer->setContentSize({200, 50});
		playerBContainer->setID("player-b-container"_spr);
		playerBContainer->addChild(cubeB);
		playerBContainer->addChild(playerBLabel);
		playerBContainer->updateLayout();

		playerContainer->addChild(playerBContainer);
		#pragma endregion

		playerContainer->updateLayout();
		popup->addChild(playerContainer);
		#pragma endregion

		#pragma region stats container
		#pragma region init
		auto statsContainer = CCMenu::create();
		statsContainer->setLayout(
			RowLayout::create()->setGap(5.f)
			// ->setGap(50.f)->setAxisAlignment(AxisAlignment::Center)
		);
		// statsContainer->setAnchorPoint({0.5f,0.5f});
		statsContainer->setPosition({winSize.width / 2, winSize.height / 2 - 35});
		statsContainer->setContentSize({430, 200});
		statsContainer->setID("stats-container"_spr);
		#pragma endregion
		#pragma region stats assignment
		/* 
			stars = 6
			diamonds = 13
			moons = 28
			sc = 8
			uc = 12
			demons = 5 
		*/

		std::vector <statsInfo> statsAVector {
			{0, "0"}, //creator points
			// {statsA->valueForKey("?")->intValue(), statsA->valueForKey("?")->getCString()},
			{statsA->valueForKey("5")->intValue(), statsA->valueForKey("5")->getCString()},
			{statsA->valueForKey("12")->intValue(), statsA->valueForKey("12")->getCString()},
			{statsA->valueForKey("8")->intValue(), statsA->valueForKey("8")->getCString()},
			{statsA->valueForKey("28")->intValue(), statsA->valueForKey("28")->getCString()},
			{statsA->valueForKey("13")->intValue(), statsA->valueForKey("13")->getCString()},
			{statsA->valueForKey("6")->intValue(), statsA->valueForKey("6")->getCString()}
		};
		std::vector <statsInfo> statsBVector {
			{statsB->m_creatorPoints, std::to_string(statsB->m_creatorPoints)},
			{statsB->m_demons, std::to_string(statsB->m_demons)},
			{statsB->m_userCoins, std::to_string(statsB->m_userCoins)},
			{statsB->m_secretCoins, std::to_string(statsB->m_secretCoins)},
			{statsB->m_moons, std::to_string(statsB->m_moons)},
			{statsB->m_diamonds, std::to_string(statsB->m_diamonds)},
			{statsB->m_stars, std::to_string(statsB->m_stars)}
		};
		#pragma endregion
		
		#pragma region player A stats
		auto playerAStatsContainer = CCNode::create();
		playerAStatsContainer->setLayout(
			ColumnLayout::create()
			->setGap(13.f)
			->setAxisAlignment(AxisAlignment::Even)
			->setCrossAxisLineAlignment(AxisAlignment::End)
			// ->setGap(50.f)->setAxisAlignment(AxisAlignment::Center)
		);
		playerAStatsContainer->setContentSize({180, 200});
		playerAStatsContainer->setID("player-A-stats-container"_spr);

		for (size_t i = 0; i < statsAVector.size(); i++){
			CCLabelBMFont* label;
			if (statsAVector[i].value > statsBVector[i].value){
				statsAVector[i].str += "(+" + std::to_string(statsAVector[i].value - statsBVector[i].value) + ") ";
				label = CCLabelBMFont::create(statsAVector[i].str.c_str(), "goldFont.fnt");
			} else {
				label = CCLabelBMFont::create(statsAVector[i].str.c_str(), "bigFont.fnt");
			}

			playerAStatsContainer->addChild(label);
		}

		// statsVector.push_back(statsA->valueForKey("6")->intValue());
		// statsVector.push_back(statsA->valueForKey("5")->intValue());
		// statsVector.push_back(statsA->valueForKey("4")->intValue());
		// statsVector.push_back(statsA->valueForKey("3")->intValue());
		// statsVector.push_back(statsA->valueForKey("2")->intValue());
		// statsVector.push_back(statsA->valueForKey("1")->intValue());
		// statsVector.push_back(statsA->valueForKey("0")->intValue());
		
		// for (int stat : statsVector){
		// 	auto playerAStatsLabel = CCLabelBMFont::create(std::to_string(stat).c_str(),"bigFont.fnt");
		// 	playerAStatsContainer->addChild(playerAStatsLabel);
		// }

		playerAStatsContainer->updateLayout();
		#pragma endregion


		#pragma region player B stats
		auto playerBStatsContainer = CCNode::create();
		playerBStatsContainer->setLayout(
			ColumnLayout::create()
			->setGap(13.f)
			->setAxisAlignment(AxisAlignment::Even)
			->setCrossAxisLineAlignment(AxisAlignment::Start)
			// ->setGap(50.f)->setAxisAlignment(AxisAlignment::Center)
		);
		playerBStatsContainer->setContentSize({180, 200});
		playerBStatsContainer->setID("player-B-stats-container"_spr);

		for (size_t i = 0; i < statsBVector.size(); i++){
			CCLabelBMFont* label;
			if (statsBVector[i].value > statsAVector[i].value){
				statsBVector[i].str += " (+" + std::to_string(statsBVector[i].value - statsAVector[i].value) + ")";
				label = CCLabelBMFont::create(statsBVector[i].str.c_str(), "goldFont.fnt");
			} else {
				label = CCLabelBMFont::create(statsBVector[i].str.c_str(), "bigFont.fnt");
			}

			playerBStatsContainer->addChild(label);
		}

		// statsVector.push_back(statsB->m_creatorPoints);
		// statsVector.push_back(statsB->m_demons);
		// statsVector.push_back(statsB->m_userCoins);
		// statsVector.push_back(statsB->m_secretCoins);
		// statsVector.push_back(statsB->m_moons);
		// statsVector.push_back(statsB->m_diamonds);
		// statsVector.push_back(statsB->m_stars);
		
		// for (int stat : statsVector){
		// 	auto playerBStatsLabel = CCLabelBMFont::create(std::to_string(stat).c_str(),"bigFont.fnt");
		// 	playerBStatsContainer->addChild(playerBStatsLabel);
		// }
		
		playerBStatsContainer->updateLayout();
		#pragma endregion

		#pragma region stats icons
		auto statsIconContainer = CCNode::create();
		statsIconContainer->setLayout(
			ColumnLayout::create()->setGap(6.f)->setAxisAlignment(AxisAlignment::Center)
			// ->setAxisAlignment(AxisAlignment::Center)
		);
		statsIconContainer->setContentSize({50, 200});
		statsIconContainer->setID("stats-icon-container"_spr);

		for (int i = 6; i >= 0; i--){
			auto statsIcon = CCSprite::createWithSpriteFrameName(getStatsIconSpriteString(i).c_str());
			statsIcon->setID(fmt::format("{}-stats", i));
			statsIconContainer->addChild(statsIcon);
		}

		statsIconContainer->updateLayout();
		#pragma endregion
		
		statsContainer->addChild(playerAStatsContainer);
		statsContainer->addChild(statsIconContainer);
		statsContainer->addChild(playerBStatsContainer);

		statsContainer->updateLayout();
		popup->addChild(statsContainer);
		#pragma endregion

		popup->m_scene = this;
		popup->show();
	};

	#pragma region init yada yada
	bool init(int id, bool a2){
		if (!ProfilePage::init(id, a2)) return false;

		m_fields->m_gameManager = GameManager::sharedState();
		return true;
	}

	virtual void loadPageFromUserInfo(GJUserScore* p0){
		ProfilePage::loadPageFromUserInfo(p0);
		
		if (p0->m_userID != m_fields->m_gameManager->m_playerUserID){
			auto mainMenu = this->getChildByIDRecursive("main-menu");
			auto spr = ButtonSprite::create("VS");
			spr->setScale(0.7f);

			auto btn = CCMenuItemSpriteExtra::create(
				spr,
				this,
				menu_selector(CompStatsProfilePage::onCompareButton)
			);

			btn->setPosition({15, -190});
			btn->setUserObject(p0);
			mainMenu->addChild(btn);
		}
	}
	#pragma endregion
};

std::string getStatsIconSpriteString(int i = 0){
	switch (i){
	case 0:
		return "GJ_starsIcon_001.png";
		break;
	case 1:
		return "GJ_diamondsIcon_001.png";
		break;
	case 2:
		return "GJ_moonsIcon_001.png";
		break;
	case 3:
		return "GJ_coinsIcon_001.png";
		break;
	case 4:
		return "GJ_coinsIcon2_001.png";
		break;
	case 5:
		return "GJ_demonIcon_001.png";
		break;
	case 6:
		return "GJ_hammerIcon_001.png";
		break;
	default:
		break;
	}
}