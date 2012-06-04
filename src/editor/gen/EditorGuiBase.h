///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __EditorGuiBase__
#define __EditorGuiBase__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_ENTITYIDFIELD 1000
#define ID_COMPLIST 1001
#define ID_BUTNEWCOMP 1002
#define ID_BUTREMCOMP 1003
#define ID_COMPIDFIELD 1004
#define ID_PROPLIST 1005
#define ID_BUTNEWPROP 1006
#define ID_BUTEDITPROP 1007
#define ID_BUTREMPROP 1008

///////////////////////////////////////////////////////////////////////////////
/// Class EditorFrameBase
///////////////////////////////////////////////////////////////////////////////
class EditorFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxToolBar* m_toolBar;
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menuFile;
		wxMenu* m_menuHelp;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_glpanel;
		wxPanel* m_sidePanel;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_entityPanel;
		wxStaticText* m_entityLabel;
		wxTextCtrl* m_entityIdField;
		wxListCtrl* m_compList;
		wxButton* m_butNewComp;
		wxButton* m_butRemoveComp;
		wxPanel* m_compPanel;
		wxStaticText* m_compLabel;
		wxTextCtrl* m_compIdField;
		wxListCtrl* m_propList;
		wxButton* m_butNewProp;
		wxButton* m_butEditProp;
		wxButton* m_butRemoveProp;
		wxPanel* m_propWarningPanel;
		wxStaticText* m_enityOk;
	
	public:
		
		EditorFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("AstroAttack Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,526 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~EditorFrameBase();
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 403 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( EditorFrameBase::m_splitter1OnIdle ), NULL, this );
		}
		
		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 0 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( EditorFrameBase::m_splitter2OnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NewCompDialog
///////////////////////////////////////////////////////////////////////////////
class NewCompDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText14;
		wxChoice* m_compChoice;
		wxStaticText* m_staticText15;
		wxTextCtrl* m_textCtrl13;
		
		wxButton* m_button6;
		wxButton* m_button7;
		
	
	public:
		
		NewCompDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create New Component"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~NewCompDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NewPropDialogBase
///////////////////////////////////////////////////////////////////////////////
class NewPropDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText4;
		wxTextCtrl* m_keyText;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_valueText;
		
		wxButton* m_button6;
		wxButton* m_button7;
		
	
	public:
		
		NewPropDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create New Property"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~NewPropDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EditPropDialogBase
///////////////////////////////////////////////////////////////////////////////
class EditPropDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText4;
		wxStaticText* m_keyLabel;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_valueText;
		
		wxButton* m_button6;
		wxButton* m_button7;
		
	
	public:
		
		EditPropDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Edit Property"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~EditPropDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel3;
		wxStaticText* m_staticText2;
		wxButton* m_butOk;
	
	public:
		
		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About AAEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~AboutDialog();
	
};

#endif //__EditorGuiBase__
