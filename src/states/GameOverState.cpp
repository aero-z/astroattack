/*
 * GameOverState.cpp
 * This file is part of Astro Attack
 * Copyright 2011 Christian Zommerfelds
 */

// GameOverState.h für mehr Informationen

#include "../GNU_config.h" // GNU Compiler-Konfiguration einbeziehen (für Linux Systeme)
#include "GameOverState.h"
#include "PlayingState.h"
#include "../Renderer.h"
#include "../GameApp.h"
#include "../Input.h"

#include <boost/make_shared.hpp>

// eindeutige ID
const StateIdType GameOverState::stateId = "GameOverState";

GameOverState::GameOverState( SubSystems& subSystems, std::string stringToShow, std::string levelFileName )
: GameState( subSystems ),
  m_stringToShow ( stringToShow ),
  m_levelFileName ( levelFileName )
{
}

void GameOverState::Init()        // State starten
{
    // GUI modus (Graphical User Interface)
    GetSubSystems().renderer->SetMatrix(RenderSubSystem::GUI);
}

void GameOverState::Cleanup()     // State abbrechen
{
}

void GameOverState::Pause()       // State anhalten
{
}

void GameOverState::Resume()      // State wiederaufnehmen
{
}

void GameOverState::Update()      // Spiel aktualisieren
{
    if ( GetSubSystems().input->KeyState( Enter ) )
    {
        boost::shared_ptr<PlayingState> playState (new PlayingState(GetSubSystems(), m_levelFileName)); // Zum Spiel-Stadium wechseln
        GetSubSystems().stateManager->ChangeState( playState );
        return;
    }
}

void GameOverState::Frame( float /*deltaTime*/ )
{
    GetSubSystems().input->Update(); // neue Eingaben lesen
}

void GameOverState::Draw( float /*accumulator*/ )        // Spiel zeichnen
{
    RenderSubSystem* pRenderer = GetSubSystems().renderer.get();

    pRenderer->DrawOverlay( cBackgroundColor[0], cBackgroundColor[1], cBackgroundColor[2], 1.0f );
    pRenderer->DrawString( m_stringToShow.c_str(), "FontW_b", 2.0f, 1.5f, AlignCenter, AlignCenter );

    pRenderer->DrawString( "ENTER: erneut starten   ESC: abbrechen", "FontW_m", 2.0f, 3.0f, AlignCenter, AlignBottom );
}
