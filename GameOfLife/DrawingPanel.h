#pragma once
#include "wx/wx.h"
#include "MainWindow.h"

class DrawingPanel :
    public wxPanel
{
public:
    DrawingPanel( wxWindow* parent, std::vector<std::vector<bool>>& _board, std::vector<std::vector<int>>& _neighbors, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);
    ~DrawingPanel();
    void SetBoard(std::vector<std::vector<bool>> _board);
    void OnPaint(wxPaintEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void SetSize(const wxSize& size);
    void SetSettings(GameSettings* _settings);
    GameSettings* settings;

    std::vector<std::vector<bool>>& gameBoard;
    std::vector<std::vector<int>>& neighbors;



    wxDECLARE_EVENT_TABLE();
};