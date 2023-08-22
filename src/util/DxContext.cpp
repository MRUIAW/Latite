#include "DxContext.h"
#include "client/Latite.h"
#include "Util.h"

D2D1_RECT_F DXContext::getRect(RectF const& rc)  {
	return D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom);
}

void DXContext::fillRectangle(RectF const& rect, d2d::Color const& color)  {
	this->brush->SetColor(color.get());
	ctx->FillRectangle(getRect(rect), brush);
}

void DXContext::drawRectangle(RectF const& rect, d2d::Color const& color, float lineThickness)  {
	this->brush->SetColor(color.get());
	ctx->DrawRectangle(getRect(rect), brush, lineThickness);
}

void DXContext::fillRectangle(RectF const& rect, ID2D1Brush* cbrush) {
	ctx->FillRectangle(getRect(rect), cbrush);
}

void DXContext::drawRectangle(RectF const& rect, ID2D1Brush* cbrush, float lineThickness) {
	ctx->DrawRectangle(getRect(rect), cbrush, lineThickness);
}

void DXContext::fillRoundedRectangle(RectF const& rect, d2d::Color const& color, float radius)  {
	this->brush->SetColor(color.get());
	auto rounded = D2D1::RoundedRect(getRect(rect), radius, radius);
	ctx->FillRoundedRectangle(rounded, brush);
}

void DXContext::drawRoundedRectangle(RectF irect, d2d::Color const& color, float radius, float lineThickness, OutlinePosition outPos)  {
	RectF rect = irect;
	this->brush->SetColor(color.get());
	switch (outPos) {
	case OutlinePosition::Inside:
		rect.left += (lineThickness / 2.f);
		rect.right -= (lineThickness / 2.f);
		rect.top += (lineThickness / 2.f);
		rect.bottom -= (lineThickness / 2.f);
		break;
	case OutlinePosition::Outside:
		rect.left -= lineThickness / 2.f;
		rect.right += lineThickness / 2.f;
		rect.top -= lineThickness / 2.f;
		rect.bottom += lineThickness / 2.f;
		radius += lineThickness / 2.f;
		break;
	default:
		break;
	}
	auto rc = getRect(rect);
	auto rounded = D2D1::RoundedRect(rc, radius, radius);

	ctx->DrawRoundedRectangle(rounded, brush, lineThickness);
}

void DXContext::drawRoundedRectangle(RectF irect, ID2D1Brush* cbrush, float radius, float lineThickness, OutlinePosition outPos)
{
	RectF rect = irect;
	switch (outPos) {
	case OutlinePosition::Inside:
		rect.left += (lineThickness / 2.f);
		rect.right -= (lineThickness / 2.f);
		rect.top += (lineThickness / 2.f);
		rect.bottom -= (lineThickness / 2.f);
		break;
	case OutlinePosition::Outside:
		rect.left -= lineThickness / 2.f;
		rect.right += lineThickness / 2.f;
		rect.top -= lineThickness / 2.f;
		rect.bottom += lineThickness / 2.f;
		radius += lineThickness / 2.f;
		break;
	default:
		break;
	}
	auto rc = getRect(rect);
	auto rounded = D2D1::RoundedRect(rc, radius, radius);

	ctx->DrawRoundedRectangle(rounded, cbrush, lineThickness);
}

void DXContext::fillRoundedRectangle(RectF const& rect, ID2D1Brush* cbrush, float radius)
{
	auto rc = getRect(rect);
	auto rounded = D2D1::RoundedRect(rc, radius, radius);

	ctx->FillRoundedRectangle(rounded, cbrush);
}

void DXContext::drawGaussianBlur(float intensity)  {
	ID2D1Effect* gaussianBlurEffect = Latite::getRenderer().getBlurEffect();
	
	// maybe we might not need to flush if we dont draw anything before clickgui?
	ctx->Flush();
	auto bitmap = Latite::getRenderer().getBlurBitmap();
	gaussianBlurEffect->SetInput(0, bitmap);
	gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, intensity);
	gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
	gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED);
	auto sz = ctx->GetPixelSize();
	auto rc = D2D1::RectF(0, 0, (float)sz.width, (float)sz.height);
	D2D1::Matrix3x2F oMat;
	//ctx->SetTransform(D2D1::Matrix3x2F::Scale(sz.width / rc.right, sz.height / rc.bottom));
	ctx->DrawImage(gaussianBlurEffect, D2D1::Point2F(0.f, 0.f), rc);
}

void DXContext::drawGaussianBlur(ID2D1Bitmap1* bmp, float intensity) {
	ID2D1Effect* gaussianBlurEffect = Latite::getRenderer().getBlurEffect();

	ctx->Flush();
	Latite::getRenderer().copyCurrentBitmap(bmp);
	gaussianBlurEffect->SetInput(0, bmp);
	gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, intensity);
	gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
	gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED);
	auto sz = ctx->GetPixelSize();
	auto rc = D2D1::RectF(0, 0, (float)sz.width, (float)sz.height);
	D2D1::Matrix3x2F oMat;
	//ctx->SetTransform(D2D1::Matrix3x2F::Scale(sz.width / rc.right, sz.height / rc.bottom));
	ctx->DrawImage(gaussianBlurEffect, D2D1::Point2F(0.f, 0.f), rc);
}

void DXContext::drawText(RectF const& rc, std::wstring const& ws, d2d::Color const& color, Renderer::FontSelection font, float size, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment)  {
	ComPtr<IDWriteTextFormat> fmt = Latite::getRenderer().getTextFormat(font);
	brush->SetColor(color.get());
	ComPtr<IDWriteTextLayout> layout;
	if (SUCCEEDED(this->factory->CreateTextLayout(ws.c_str(), static_cast<uint32_t>(ws.size()),
		fmt.Get(), rc.getWidth(), rc.getHeight(),
		layout.GetAddressOf()))) {
		DWRITE_TEXT_RANGE range;
		range.startPosition = 0;
		range.length = static_cast<UINT32>(ws.size());
		layout->SetFontSize(size, range);
		layout->SetTextAlignment(alignment);
		layout->SetParagraphAlignment(verticalAlignment);
		this->ctx->DrawTextLayout({ rc.getPos().x, rc.getPos().y }, layout.Get(),
			brush);
	}
}

Vec2 DXContext::getTextSize(std::wstring const& ws, Renderer::FontSelection font, float size, bool tw) {
	ComPtr<IDWriteTextFormat> fmt = Latite::getRenderer().getTextFormat(font);
	auto ss = ctx->GetPixelSize();
	ComPtr<IDWriteTextLayout> layout;
	if (SUCCEEDED(this->factory->CreateTextLayout(ws.c_str(), static_cast<uint32_t>(ws.size()),
		fmt.Get(), (float)ss.width, (float)ss.height,
		layout.GetAddressOf()))) {
		DWRITE_TEXT_RANGE range;
		range.startPosition = 0;
		range.length = static_cast<UINT32>(ws.size());
		layout->SetFontSize(size, range);
		layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		DWRITE_TEXT_METRICS textMetrics;
		DWRITE_OVERHANG_METRICS overhangs;
		layout->GetMetrics(&textMetrics);
		layout->GetOverhangMetrics(&overhangs);
		
		float width = tw ? textMetrics.widthIncludingTrailingWhitespace : ((textMetrics.layoutWidth) + overhangs.right) - (textMetrics.left - overhangs.left);
		float height = textMetrics.height;
		
		return Vec2(width, height);
	}
	return {};
}

d2d::Rect DXContext::getTextRect(std::wstring const& ws, Renderer::FontSelection font, float size, float pad) {
	ComPtr<IDWriteTextFormat> fmt = Latite::getRenderer().getTextFormat(font);
	auto ss = ctx->GetPixelSize();
	ComPtr<IDWriteTextLayout> layout;
	if (SUCCEEDED(this->factory->CreateTextLayout(ws.c_str(), static_cast<uint32_t>(ws.size()),
		fmt.Get(), static_cast<float>(ss.width), static_cast<float>(ss.height),
		layout.GetAddressOf()))) {
		DWRITE_TEXT_RANGE range;
		range.startPosition = 0;
		range.length = static_cast<UINT32>(ws.size());
		layout->SetFontSize(size, range);
		layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		DWRITE_TEXT_METRICS metrics;
		DWRITE_OVERHANG_METRICS overhangs;
		layout->GetMetrics(&metrics);
		layout->GetOverhangMetrics(&overhangs);

		return {
			metrics.left - overhangs.left - pad, metrics.top - overhangs.top - pad,
			metrics.layoutWidth + overhangs.right + pad, metrics.layoutHeight + overhangs.bottom + pad
		};
	}
	return {};
}

DXContext::DXContext() : brush(Latite::getRenderer().getSolidBrush()), ctx(Latite::getRenderer().getDeviceContext()), factory(Latite::getRenderer().getDWriteFactory())  {
}