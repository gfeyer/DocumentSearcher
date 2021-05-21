///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "noname.h"

///////////////////////////////////////////////////////////////////////////

UIFrame::UIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	auinotebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	bSizer8->Add( auinotebook, 1, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizer8 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	File = new wxMenu();
	wxMenuItem* gui_menu_file_quit;
	gui_menu_file_quit = new wxMenuItem( File, wxID_ANY, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	File->Append( gui_menu_file_quit );

	m_menubar1->Append( File, wxT("File") );

	Index = new wxMenu();
	wxMenuItem* gui_menu_index_new;
	gui_menu_index_new = new wxMenuItem( Index, wxID_ANY, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	Index->Append( gui_menu_index_new );

	wxMenuItem* gui_menu_index_view;
	gui_menu_index_view = new wxMenuItem( Index, wxID_ANY, wxString( wxT("View") ) , wxEmptyString, wxITEM_NORMAL );
	Index->Append( gui_menu_index_view );

	m_menubar1->Append( Index, wxT("Index") );

	Help = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( Help, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	Help->Append( m_menuItem3 );

	m_menubar1->Append( Help, wxT("Help") );

	this->SetMenuBar( m_menubar1 );


	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( UIFrame::OnClose ) );
	File->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UIFrame::OnSelectMenuQuit ), this, gui_menu_file_quit->GetId());
	Index->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UIFrame::OnSelectMenuIndexNew ), this, gui_menu_index_new->GetId());
	Index->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UIFrame::OnSelectMenuIndexView ), this, gui_menu_index_view->GetId());
	Help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UIFrame::OnAbout ), this, m_menuItem3->GetId());
}

UIFrame::~UIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( UIFrame::OnClose ) );

}

CustomMessageFrame::CustomMessageFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );

	m_panel9 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel9->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
	m_panel9->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	bSizer38->Add( m_panel9, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer38 );
	this->Layout();

	this->Centre( wxBOTH );
}

CustomMessageFrame::~CustomMessageFrame()
{
}

SearchPanel::SearchPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );

	gui_button_search = new wxButton( this, wxID_ANY, wxT("Search"), wxDefaultPosition, wxDefaultSize, 0 );
	gui_button_search->SetToolTip( wxT("execute filter") );

	bSizer19->Add( gui_button_search, 0, wxALL, 5 );

	gui_search_query = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	gui_search_query->Append( wxT(".request") );
	gui_search_query->SetToolTip( wxT("filter jq") );

	bSizer19->Add( gui_search_query, 14, wxALL, 5 );


	bSizer14->Add( bSizer19, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );

	gui_console = new wxStaticText( this, wxID_ANY, wxT("[statistics]"), wxDefaultPosition, wxDefaultSize, 0 );
	gui_console->Wrap( -1 );
	bSizer81->Add( gui_console, 0, wxALL, 2 );


	bSizer14->Add( bSizer81, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );

	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( SearchPanel::m_splitter1OnIdle ), NULL, this );

	m_panel5 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	gui_list_view = new wxDataViewListCtrl( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES|wxDV_ROW_LINES );
	bSizer22->Add( gui_list_view, 1, wxALL|wxEXPAND, 5 );


	m_panel5->SetSizer( bSizer22 );
	m_panel5->Layout();
	bSizer22->Fit( m_panel5 );
	m_panel6 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxHORIZONTAL );

	gui_search = new wxSearchCtrl( m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	gui_search->ShowSearchButton( true );
	#endif
	gui_search->ShowCancelButton( true );
	bSizer37->Add( gui_search, 1, wxALL, 2 );


	bSizer23->Add( bSizer37, 0, wxEXPAND, 5 );

	gui_text_view = new wxStyledTextCtrl( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	gui_text_view->SetUseTabs( true );
	gui_text_view->SetTabWidth( 4 );
	gui_text_view->SetIndent( 4 );
	gui_text_view->SetTabIndents( true );
	gui_text_view->SetBackSpaceUnIndents( true );
	gui_text_view->SetViewEOL( false );
	gui_text_view->SetViewWhiteSpace( false );
	gui_text_view->SetMarginWidth( 2, 0 );
	gui_text_view->SetIndentationGuides( true );
	gui_text_view->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	gui_text_view->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	gui_text_view->SetMarginWidth( 1, 16);
	gui_text_view->SetMarginSensitive( 1, true );
	gui_text_view->SetProperty( wxT("fold"), wxT("1") );
	gui_text_view->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	gui_text_view->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	gui_text_view->SetMarginWidth( 0, gui_text_view->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	gui_text_view->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	gui_text_view->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	gui_text_view->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	gui_text_view->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	gui_text_view->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	gui_text_view->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	gui_text_view->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	gui_text_view->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	gui_text_view->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	gui_text_view->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	gui_text_view->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	bSizer23->Add( gui_text_view, 1, wxEXPAND | wxALL, 2 );


	m_panel6->SetSizer( bSizer23 );
	m_panel6->Layout();
	bSizer23->Fit( m_panel6 );
	m_splitter1->SplitVertically( m_panel5, m_panel6, 0 );
	bSizer20->Add( m_splitter1, 1, wxEXPAND, 0 );


	bSizer14->Add( bSizer20, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer14 );
	this->Layout();

	// Connect Events
	gui_button_search->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SearchPanel::OnSearch ), NULL, this );
	gui_search_query->Connect( wxEVT_KEY_UP, wxKeyEventHandler( SearchPanel::OnKeyUpFilter ), NULL, this );
	gui_list_view->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( SearchPanel::OnDoubleClick ), NULL, this );
	gui_list_view->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( SearchPanel::OnSelect ), NULL, this );
	gui_search->Connect( wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler( SearchPanel::OnDocSearchCancel ), NULL, this );
	gui_search->Connect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( SearchPanel::OnDocSearch ), NULL, this );
	gui_search->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SearchPanel::OnDocSearchPartial ), NULL, this );
}

SearchPanel::~SearchPanel()
{
	// Disconnect Events
	gui_button_search->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SearchPanel::OnSearch ), NULL, this );
	gui_search_query->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( SearchPanel::OnKeyUpFilter ), NULL, this );
	gui_list_view->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( SearchPanel::OnDoubleClick ), NULL, this );
	gui_list_view->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( SearchPanel::OnSelect ), NULL, this );
	gui_search->Disconnect( wxEVT_COMMAND_SEARCHCTRL_CANCEL_BTN, wxCommandEventHandler( SearchPanel::OnDocSearchCancel ), NULL, this );
	gui_search->Disconnect( wxEVT_COMMAND_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler( SearchPanel::OnDocSearch ), NULL, this );
	gui_search->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SearchPanel::OnDocSearchPartial ), NULL, this );

}

FilterFrame::FilterFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText13 = new wxStaticText( m_panel4, wxID_ANY, wxT("Directory to index"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer31->Add( m_staticText13, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	gui_source_dir = new wxDirPickerCtrl( m_panel4, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer31->Add( gui_source_dir, 5, wxALL, 5 );


	bSizer30->Add( bSizer31, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer311;
	bSizer311 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText131 = new wxStaticText( m_panel4, wxID_ANY, wxT("Save new index to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	bSizer311->Add( m_staticText131, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	gui_index_dir = new wxDirPickerCtrl( m_panel4, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer311->Add( gui_index_dir, 5, wxALL, 5 );


	bSizer30->Add( bSizer311, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer3111;
	bSizer3111 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1311 = new wxStaticText( m_panel4, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1311->Wrap( -1 );
	bSizer3111->Add( m_staticText1311, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	gui_name = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3111->Add( gui_name, 5, wxALL, 5 );


	bSizer30->Add( bSizer3111, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer31111;
	bSizer31111 = new wxBoxSizer( wxVERTICAL );

	gui_button_start = new wxButton( m_panel4, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31111->Add( gui_button_start, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP|wxALL, 5 );


	bSizer30->Add( bSizer31111, 0, wxEXPAND, 5 );

	gui_console = new wxStyledTextCtrl( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	gui_console->SetUseTabs( true );
	gui_console->SetTabWidth( 4 );
	gui_console->SetIndent( 4 );
	gui_console->SetTabIndents( true );
	gui_console->SetBackSpaceUnIndents( true );
	gui_console->SetViewEOL( false );
	gui_console->SetViewWhiteSpace( false );
	gui_console->SetMarginWidth( 2, 0 );
	gui_console->SetIndentationGuides( true );
	gui_console->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	gui_console->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	gui_console->SetMarginWidth( 1, 16);
	gui_console->SetMarginSensitive( 1, true );
	gui_console->SetProperty( wxT("fold"), wxT("1") );
	gui_console->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	gui_console->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	gui_console->SetMarginWidth( 0, gui_console->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	gui_console->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	gui_console->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	gui_console->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	gui_console->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	gui_console->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	gui_console->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	gui_console->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	gui_console->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	gui_console->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	gui_console->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	gui_console->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	bSizer30->Add( gui_console, 1, wxEXPAND | wxALL, 5 );


	m_panel4->SetSizer( bSizer30 );
	m_panel4->Layout();
	bSizer30->Fit( m_panel4 );
	bSizer10->Add( m_panel4, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );
}

FilterFrame::~FilterFrame()
{
}
