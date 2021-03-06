/*
 * MainMenuState.cpp
 * This file is part of Astro Attack
 * Copyright (c) 2014 Christian Zommerfelds
 */


#include "MainMenuState.h"

#include "PlayingState.h"
#include "SlideShowState.h"

#include "game/Gui.h"
#include "game/Configuration.h"
#include "game/GameApp.h"
#include "game/Input.h"
#include "game/main.h"

#include "common/Renderer.h"
#include "common/Vector2D.h"
#include "common/Texture.h"
#include "common/Sound.h"
#include "common/Logger.h"
#include "common/DataLoader.h"
#include "common/Foreach.h"

#include "contrib/pugixml/pugixml.hpp"
#include "contrib/pugixml/foreach.hpp"

#include <cmath>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

const GameStateId MainMenuState::STATE_ID = "MainMenuState";

namespace {
const std::string cLevelSequenceFileName = "data/levelSequence.xml";
const std::string cMenuGraphicsFileName = "data/graphicsMenu.info"; // hier sind Menügrafiken angegeben

const std::string menuNames[] =
{ 
    "MainMenu",
    "PlayMenu",
    "CreditsMenu",
    "OptionsMenu",
};

const float cTitleVertexCoord[8] = { 0.1f, 0.07f,
                                     0.1f, 0.25f,
                                     0.9f, 0.25f,
                                     0.9f, 0.07f };
const float cTitleIntensitySpeed = 0.006f;
}

MainMenuState::MainMenuState( SubSystems& subSystems, SubMenu startingSubMenu )
: GameState(subSystems),
  m_titleIntensityPhase (0.0f),
  m_subMenu (startingSubMenu),
  m_wantToQuit (false),
  m_restart (false),
  m_goToPlay (false),
  m_goToSlideShow (false),
  m_newWidth (-1),
  m_newHeight (-1),
  m_menuResources ()
{}

void MainMenuState::init()        // State starten
{
    using boost::make_shared;

    log(Info) << "Loading menu... ";
    //GetSubSystems().renderer.DisplayLoadingScreen();
    
    // Grafiken aus XML-Datei laden
    m_menuResources = DataLoader::loadGraphics(cMenuGraphicsFileName, &getSubSystems().renderer.getTextureManager(), NULL, NULL, (TexQuality) gConfig.get<int>("TexQuality"));

    getSubSystems().sound.loadMusic( "data/Music/ParagonX9___Chaoz_C.ogg", "menuMusic" );
    getSubSystems().sound.loadSound( "data/Sounds/Single click stab with delay_Nightingale Music Productions_12046.wav", "mouseclick" );
    getSubSystems().sound.loadSound( "data/Sounds/FOLEY TOY SINGLE CLICK 01.wav", "mouseover" );
    getSubSystems().sound.playMusic( "menuMusic", true, 0 );

    // GUI modus (Graphical User Interface)
    getSubSystems().renderer.setMatrix(RenderSystem::GUI);

    ///////////// GUI ////////////
    getSubSystems().gui.hideGroup( menuNames[0] );
    getSubSystems().gui.hideGroup( menuNames[1] );
    getSubSystems().gui.hideGroup( menuNames[2] );
    getSubSystems().gui.hideGroup( menuNames[3] );
    getSubSystems().gui.showGroup( menuNames[m_subMenu] );

    // *** Hauptbildschirm ***
    getSubSystems().gui.addWidget( menuNames[Main], make_shared<WidgetLabel>( 0.14f, 0.9f, "Welcome to AstroAttack! [v" GAME_VERSION "]", getSubSystems().renderer.getFontManager() ) );


    float x = 0.35f;
    float y = 0.4f;
    float w=0.18f,h=0.05f,w2=0.3f;
    float yspacing = 0.07f;
    float yspacing2 = 0.04f;

    getSubSystems().gui.addWidget( menuNames[Main], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "Play", boost::bind( &MainMenuState::onPressedButPlay, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    y += yspacing;
    getSubSystems().gui.addWidget( menuNames[Main], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "About", boost::bind( &MainMenuState::onPressedButCredits, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    y += yspacing;
    getSubSystems().gui.addWidget( menuNames[Main], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "Options", boost::bind( &MainMenuState::onPressedButOptions, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    y += yspacing;
    getSubSystems().gui.addWidget( menuNames[Main], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "Quit", boost::bind( &MainMenuState::onPressedButExit, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );

    // *** Zurückknöpfe für Untermenüs ***
    const float back_x=0.03f;
    const float back_y=0.92f;
    getSubSystems().gui.addWidget( menuNames[1], make_shared<WidgetButton>( Rect(back_x,back_x+w,back_y,back_y+h), "Back", boost::bind( &MainMenuState::onPressedButBack, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    getSubSystems().gui.addWidget( menuNames[2], make_shared<WidgetButton>( Rect(back_x,back_x+w,back_y,back_y+h), "Back", boost::bind( &MainMenuState::onPressedButBack, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    //GetSubSystems().gui.InsertWidget( menuNames[3], boost::shared_ptr<Widget>(new WidgetButton( Rect(back_x,back_x+w,back_y,back_y+h), "Back", boost::bind( &MainMenuState::CallbackButBack, this ), boost::bind( &MainMenuState::CallbackSound, this ) )) );

    // *** Spielen Menü ***
    getSubSystems().gui.addWidget( menuNames[Play], make_shared<WidgetLabel>(  0.14f, 0.21f, "Pick a level:", getSubSystems().renderer.getFontManager() ) );

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(cLevelSequenceFileName.c_str());
    if (!result)
    {
        log(Error) << "[ Error parsing file '" << cLevelSequenceFileName << "' at offset " << result.offset << "!\nError description: " << result.description() << " ]\n\n";
        return;
    }

    x=0.35f;
    y=0.2f;
    foreach(pugi::xml_node node, doc.first_child())
    {
        std::string caption = node.attribute("name").value();
        std::string value = node.name();
        std::string file = node.attribute("file").value();
        if ( value == "level" )
            getSubSystems().gui.addWidget( menuNames[Play], boost::make_shared<WidgetButton>( Rect(x,x+w2,y,y+h), caption, boost::bind( &MainMenuState::onPressedOpenLevel, this, file ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
        else if ( value == "slides" )
            getSubSystems().gui.addWidget( menuNames[Play], boost::make_shared<WidgetButton>( Rect(x,x+w2,y,y+h), caption, boost::bind( &MainMenuState::onPressedOpenSlideShow, this, file ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
		// NOTE: needs to specify boost:: because of ADL (see http://www.gamedev.net/topic/619313-visual-studio-namespace-problem-ambiguous-call-to-boostmake-shared/)
        y += yspacing;
    }

    // *** Über ***
    x=0.15f;
    y=0.25f;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              "== AstroAttack " GAME_VERSION " ==", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing2;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              "by Christian Zommerfelds", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              "* Music *", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing2;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              " - Dj Mitch (SRT-M1tch) -> Menu", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing2;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              " - Sir LardyLarLar AKA Robin (FattysLoyalKnight) -> Intro", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing2;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              " - sr4cld - 4clD -> Spiel", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              "* Sound *", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing2;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              " - soundsnap.com", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              "* Libraries *", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing2;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              " - OpenGL, SDL, DevIL, FTGL, SDL_mixer, Box2D, Boost", getSubSystems().renderer.getFontManager() ) );
    y+=yspacing;
    getSubSystems().gui.addWidget( menuNames[Credits], make_shared<WidgetLabel>( x, y,
                              "Thanks for playing!", getSubSystems().renderer.getFontManager() ) );

    // *** Optionen ***
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetLabel>( 0.3f, 0.1f, "Screen resolution:", getSubSystems().renderer.getFontManager() ) );
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetLabel>( 0.1f, 0.8f, "Please change \"config.info\" for more options.", getSubSystems().renderer.getFontManager() ) );
    
    x=0.3f;
    y=0.92f;
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "Cancel", boost::bind( &MainMenuState::onPressedButDiscardConfig, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    x=yspacing;
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "Apply", boost::bind( &MainMenuState::onPressedButApplyConfig, this ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    
    // Auflösungen
    x=0.03f;
    y=0.15f;
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "800x600", boost::bind( &MainMenuState::onPressedResolution, this, 800, 600 ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    x += w+0.05f;
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "1024x768", boost::bind( &MainMenuState::onPressedResolution, this, 1024, 768 ), boost::bind( &MainMenuState::onPressedSound, this ) ) );
    x += w+0.05f;
    getSubSystems().gui.addWidget( menuNames[Options], make_shared<WidgetButton>( Rect(x,x+w,y,y+h), "1280x1024", boost::bind( &MainMenuState::onPressedResolution, this, 1280, 1024 ), boost::bind( &MainMenuState::onPressedSound, this ) ) );

    
    log(Info) << "[ Done ]\n";
}

void MainMenuState::cleanup()     // State abbrechen
{
    /*if ( m_appliedConfig == false )
        gAaConfig.discardConfig();*/
    getSubSystems().sound.stopMusic( 300 );
    getSubSystems().sound.freeSound( "mouseover" );
    getSubSystems().sound.freeSound( "mouseclick" );

    // Grafiken wieder freisetzen
     DataLoader::unLoadGraphics( m_menuResources, &getSubSystems().renderer.getTextureManager(), NULL, NULL );

    getSubSystems().gui.clear();

    getSubSystems().sound.freeMusic( "menuMusic" );
}

void MainMenuState::pause()       // State anhalten
{
}

void MainMenuState::resume()      // State wiederaufnehmen
{
}

void MainMenuState::update()      // Spiel aktualisieren
{
    if ( m_wantToQuit )
    {
        getSubSystems().events.quitGame.fire(m_restart);
        return;
    }
    if ( m_goToPlay )
    {
        boost::shared_ptr<PlayingState> playingState ( new PlayingState(getSubSystems(), m_fileNameToOpen) );
        getSubSystems().stateManager.changeState( playingState );
        return;
    }
    if ( m_goToSlideShow )
    {
        boost::shared_ptr<SlideShowState> introSlideShowState ( new SlideShowState( getSubSystems(), m_fileNameToOpen ) );
        getSubSystems().stateManager.changeState( introSlideShowState );
        return;
    }

    // AstroAttack banner
    m_titleIntensityPhase += cTitleIntensitySpeed;
    if ( m_titleIntensityPhase > 2*cPi )
        m_titleIntensityPhase -= 2*cPi;
}

void MainMenuState::frame( float /*deltaTime*/ )
{
    getSubSystems().input.update();   // neue Eingaben lesen
    getSubSystems().gui.update();     // Benutzeroberfläche aktualisieren
}

void MainMenuState::draw( float /*accumulator*/ )        // Spiel zeichnen
{
    RenderSystem& renderer = getSubSystems().renderer;
    const Vector2D& mousePos = getSubSystems().input.getMousePos();

    // Menühintergrund zeichnen
    {
        float texCoord[8] = { 0.0f, 0.0f,
                              0.0f, 1.0f,
                              1.0f, 1.0f,
                              1.0f, 0.0f };
        float* const & vertexCoord = texCoord;
        renderer.drawTexturedQuad( texCoord, vertexCoord, "menu" );
    }

    if( m_subMenu == Main || m_subMenu == Credits )
    {
        // Titel (AstroAttack) zeichnen
        float texCoord[8] = { 0.0f, 0.0f,
                              0.0f, 1.0f,
                              1.0f, 1.0f,
                              1.0f, 0.0f };
        float titleVertexCoord[8] = {0.0f};
        float offset = sin(m_titleIntensityPhase)*0.01f;
        titleVertexCoord[0] = cTitleVertexCoord[0] - offset;
        titleVertexCoord[2] = cTitleVertexCoord[2] - offset;
        titleVertexCoord[4] = cTitleVertexCoord[4] + offset;
        titleVertexCoord[6] = cTitleVertexCoord[6] + offset;
        titleVertexCoord[1] = cTitleVertexCoord[1] - offset;
        titleVertexCoord[3] = cTitleVertexCoord[3] + offset;
        titleVertexCoord[5] = cTitleVertexCoord[5] + offset;
        titleVertexCoord[7] = cTitleVertexCoord[7] - offset;
        float intensity = (sin(m_titleIntensityPhase) / 2.0f + 0.5f) * 0.6f + 0.3f;
        renderer.drawTexturedQuad( texCoord, titleVertexCoord, "title", false, intensity );
    }

    // testing
    //renderer.drawString("Hallo", "FontW_b", 0.0f, 3.0f, AlignLeft, AlignBottom);

    getSubSystems().gui.draw();

    // Cursor (Mauszeiger) zeichnen
    {
        float texCoord[8] = { 0.0f, 0.0f,
                              0.0f, 1.0f,
                              1.0f, 1.0f,
                              1.0f, 0.0f };

        float aspectRatio = (float(gConfig.get<int>("ScreenWidth")))/gConfig.get<int>("ScreenHeight");

        float w = 0.035;
        float h = 0.035f * aspectRatio;
        float vertexCoord[8] = { mousePos.x     ,  mousePos.y     ,
                                 mousePos.x     , (mousePos.y + h),
                                (mousePos.x + w), (mousePos.y + h),
                                (mousePos.x + w),  mousePos.y      };
        renderer.drawTexturedQuad( texCoord, vertexCoord, "_cursor" );
    }
}

void MainMenuState::onPressedButPlay()
{
    m_subMenu=Play;
    getSubSystems().gui.hideGroup( menuNames[0] );
    getSubSystems().gui.showGroup( menuNames[1]);
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedButExit()
{
    m_wantToQuit = true;
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedButCredits()
{
    m_subMenu=Credits;
    getSubSystems().gui.hideGroup( menuNames[0] );
    getSubSystems().gui.showGroup( menuNames[2] );
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedButOptions()
{
    m_subMenu=Options;
    getSubSystems().gui.hideGroup( menuNames[0] );
    getSubSystems().gui.showGroup( menuNames[3] );
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedButBack()
{

    getSubSystems().gui.hideGroup( menuNames[m_subMenu] );
    m_subMenu=Main;
    getSubSystems().gui.showGroup( menuNames[0] );
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedOpenLevel( const std::string& filename )
{
    m_goToPlay = true;
    m_fileNameToOpen = filename;
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedOpenSlideShow( const std::string& filename )
{
    m_goToSlideShow = true;
    m_fileNameToOpen = filename;
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedSound()
{
    getSubSystems().sound.playSound( "mouseover" );
}

void MainMenuState::onPressedResolution( int w, int h )
{
    m_newWidth = w;
    m_newHeight = h;
    //gAaConfig.setInt("ScreenWidth",w);
    //gAaConfig.setInt("ScreenHeight",h);
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedButApplyConfig()
{
    //m_appliedConfig = true;
    if (m_newWidth != -1 && m_newHeight != -1)
    {
        gConfig.put("ScreenWidth", m_newWidth);
        gConfig.put("ScreenHeight", m_newHeight);
        m_restart = true;
        m_wantToQuit = true;
    }
    getSubSystems().sound.playSound( "mouseclick" );
}

void MainMenuState::onPressedButDiscardConfig()
{
    //gAaConfig.discardConfig();
    onPressedButBack();
    getSubSystems().sound.playSound( "mouseclick" );
}
