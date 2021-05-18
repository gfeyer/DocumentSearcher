///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/gdicmn.h>
#include <wx/aui/auibook.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/dataview.h>
#include <wx/srchctrl.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class UIFrame
///////////////////////////////////////////////////////////////////////////////
class UIFrame : public wxFrame
{
	private:

	protected:
		wxAuiNotebook* auinotebook;
		wxMenuBar* m_menubar1;
		wxMenu* File;
		wxMenu* Filters;
		wxMenu* Help;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectFiltersSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuFiltersView( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }


	public:

		UIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("KafkaDesktopClient X.Y"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~UIFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CustomMessageFrame
///////////////////////////////////////////////////////////////////////////////
class CustomMessageFrame : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel9;

	public:

		CustomMessageFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,400 ), long style = wxFRAME_FLOAT_ON_PARENT|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );

		~CustomMessageFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class InspectorPanel
///////////////////////////////////////////////////////////////////////////////
class InspectorPanel : public wxPanel
{
	private:

	protected:
		wxButton* gui_button_run;
		wxSpinCtrl* gui_buffer_size;
		wxComboBox* gui_topic;
		wxComboBox* gui_brokers;
		wxButton* gui_button_filter;
		wxComboBox* gui_filter;
		wxStaticText* gui_console;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel5;
		wxDataViewListCtrl* gui_list_view;
		wxPanel* m_panel6;
		wxSearchCtrl* gui_search;
		wxStyledTextCtrl* gui_text_view;

		// Virtual event handlers, overide them in your derived class
		virtual void OnRun( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFilter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyUpFilter( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnSelect( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnSearchCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSearch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSearchPartial( wxCommandEvent& event ) { event.Skip(); }


	public:

		InspectorPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,600 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~InspectorPanel();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 0 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( InspectorPanel::m_splitter1OnIdle ), NULL, this );
		}

};

