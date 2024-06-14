#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()


DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& _board, std::vector<std::vector<int>>& _neighbors , wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name) , gameBoard(_board), neighbors(_neighbors) {
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel() {

}

void DrawingPanel::OnPaint(wxPaintEvent& event) {
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* ctx = wxGraphicsContext::Create(dc);
	wxWindow* _window = this->GetParent();

	if (!ctx) return;

	if (settings->displayGrid) {
		ctx->SetPen(*wxBLACK);
	}
	else {
		ctx->SetPen(wxTransparentColor);
	}
	ctx->SetBrush(*wxWHITE);

	int cellWidth = GetSize().x / settings->gridSize;
	int cellHeight = GetSize().y / settings->gridSize;

	ctx->DrawRectangle(0, 0, cellWidth, cellHeight);

	int rectangleAmount = settings->gridSize * settings->gridSize;

	int column = 0;
	int row = -1;

	for (int i = 0; i < rectangleAmount; i++) {
		if (i % settings->gridSize == 0) {
			column = 0;
			row += 1;
		}
		else {
			column++;
		}

		if (gameBoard[row][column]) {
			ctx->SetBrush(settings->GetColor(ColorType::Living));
		}
		else {
			ctx->SetBrush(settings->GetColor(ColorType::Dead));
		}

		if (settings->showNeighborCount) {
			ctx->SetFont(wxFontInfo(16), *wxRED);
			wxString neighborString = std::to_string(neighbors[row][column]);
			wxDouble textWidth, textHeight;
			ctx->GetTextExtent(neighborString, &textWidth, &textHeight);

			wxDouble cellX = column * cellWidth + (cellWidth - textWidth) / 2.0;
			wxDouble cellY = row * cellHeight + (cellHeight - textHeight) / 2.0;

			ctx->DrawText(neighborString, cellX, cellY);
		}

		ctx->DrawRectangle(column * cellWidth, row * cellHeight, cellWidth, cellHeight);
	}
	
	delete ctx;
}

void DrawingPanel::SetSize(const wxSize& size) {
	wxPanel::SetSize(size);
	Refresh();
}

void DrawingPanel::OnMouseUp(wxMouseEvent& event) {
	int x = event.GetX();
	int y = event.GetY();

	int panelWidth = GetSize().GetWidth();
	int panelHeight = GetSize().GetHeight();

	int cellWidth = panelWidth / settings->gridSize;
	int cellHeight = panelHeight / settings->gridSize;

	int column = x / cellWidth;
	int row = y / cellHeight;

	if (!(row > settings->gridSize || column > settings->gridSize)
		|| (row > settings->gridSize && column > settings->gridSize)) {
		if (gameBoard[row][column]) {
			gameBoard[row][column] = false;
		}
		else {
			gameBoard[row][column] = true;
		}
	}

	Refresh();
}

void DrawingPanel::SetSettings(GameSettings* _settings) {
	settings = _settings;
}