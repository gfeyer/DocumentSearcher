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
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/dataview.h>
#include <wx/srchctrl.h>
#include <wx/stc/stc.h>
#include <wx/splitter.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>

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
		wxMenu* Index;
		wxMenu* Help;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuIndexNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectMenuIndexView( wxCommandEvent& event ) { event.Skip(); }
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
/// Class SearchPanel
///////////////////////////////////////////////////////////////////////////////
class SearchPanel : public wxPanel
{
	private:

	protected:
		wxButton* gui_button_search;
		wxComboBox* gui_search_query;
		wxStaticText* gui_console;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel5;
		wxDataViewListCtrl* gui_list_view;
		wxPanel* m_panel6;
		wxSearchCtrl* gui_search;
		wxStyledTextCtrl* gui_text_view;

		// Virtual event handlers, overide them in your derived class
		virtual void OnSearch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyUpFilter( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnDoubleClick( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnSelect( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnDocSearchCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDocSearch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDocSearchPartial( wxCommandEvent& event ) { event.Skip(); }


	public:

		SearchPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,600 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~SearchPanel();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 0 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( SearchPanel::m_splitter1OnIdle ), NULL, this );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class FilterFrame
///////////////////////////////////////////////////////////////////////////////
class FilterFrame : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText13;
		wxDirPickerCtrl* gui_source_dir;
		wxStaticText* m_staticText131;
		wxDirPickerCtrl* gui_index_dir;
		wxStaticText* m_staticText1311;
		wxTextCtrl* gui_name;
		wxButton* gui_button_start;
		wxStyledTextCtrl* gui_console;

		// Virtual event handlers, overide them in your derived class
		virtual void OnStart( wxCommandEvent& event ) { event.Skip(); }


	public:

		FilterFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create new index"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FilterFrame();

};

