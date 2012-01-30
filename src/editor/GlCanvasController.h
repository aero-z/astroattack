/*
 * GlCanvasController.h
 * This file is part of Astro Attack
 * Copyright 2012 Christian Zommerfelds
 */

#ifndef GLCANVAS_H
#define GLCANVASCONTROLLER_H

#include "common/Vector2D.h"
#include "common/Renderer.h"
#include "common/CameraController.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>

struct GameEvents;
class Editor;

class GlCanvasController : public wxGLCanvas
{
public:
    GlCanvasController(Editor& editor, wxWindow* parent, int* args, RenderSubSystem& renderer);
    virtual ~GlCanvasController();
    void resetCamera();

private:
    void onPaint(wxPaintEvent& evt);
    void onResize(wxSizeEvent& evt);
    void onLMouseDown(wxMouseEvent& evt);
    void onLMouseUp(wxMouseEvent& evt);
    void onRMouseDown(wxMouseEvent& evt);
    void onRMouseUp(wxMouseEvent& evt);
    void onMouseMotion(wxMouseEvent& evt);
    void onMouseLeaveWindow(wxMouseEvent& evt);
    void onMouseEnterWindow(wxMouseEvent& evt);
    void onKeyDown(wxKeyEvent& evt);

    Editor& m_editor;
    wxGLContext* m_context;
    RenderSubSystem& m_renderer;
    CameraController m_cameraController;

    Vector2D m_lastCursorPos;
    bool m_lMouseIsDown;  // whether the left mouse button is pressed and we have input focus
    bool m_mouseInWindow; // whether the mouse is inside the canvas area

    int m_initCount;

    DECLARE_EVENT_TABLE()
};

#endif