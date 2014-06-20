#include "stdafx.h"
#include "ExportInfo.h"
#include "tinyxml.h"


ExportInfo::ExportInfo(void)
{
	m_pMonitors=NULL;
	m_pCanvas=NULL;
}

void ExportInfo::SetData(const CanvasInfo* pcanvas,const std::vector<MonitorInfo>* pmonitors)
{
	m_pCanvas=pcanvas;
	m_pMonitors=pmonitors;
}
bool ExportInfo::SaveFile(const char* filename)
{
	if(!(m_pCanvas && m_pMonitors))
		return false;
	assert(filename);
	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0","UTF-8","");
	doc.InsertEndChild(decl);
	TiXmlElement canvas("ScreenCanvas");
	SetCanvas(&canvas,m_pCanvas);
	for(size_t i=0;i<m_pMonitors->size();++i)
	{
		TiXmlElement screen("Screen");
		SetScreen(&screen,&(*m_pMonitors)[i]);
		canvas.InsertEndChild(screen);
	}
	doc.InsertEndChild(canvas);
	return doc.SaveFile(filename);
}

void ExportInfo::SetCanvas(TiXmlElement* canvas,const CanvasInfo* info)
{
	assert(canvas && info);
	canvas->SetAttribute("width",info->width);
	canvas->SetAttribute("height",info->height);
	canvas->SetAttribute("horizon",info->horizon);
	canvas->SetAttribute("vertical",info->vertical);
}
void ExportInfo::SetScreen(TiXmlElement* screen,const MonitorInfo* info)
{
	assert(screen && info);
	screen->SetAttribute("id",info->m_nID);
	screen->SetAttribute("type",CString2StdString(info->m_type.m_strDescription).c_str());
	screen->SetAttribute("x",info->m_posOrig.m_posInPixel.x);
	screen->SetAttribute("y",info->m_posOrig.m_posInPixel.y);
	screen->SetAttribute("horizon",info->m_type.m_nHorizonResolution);
	screen->SetAttribute("vertical",info->m_type.m_nVerticalResolution);
	screen->SetAttribute("width",info->m_type.m_nWidth);
	screen->SetAttribute("height",info->m_type.m_nHeight);
	screen->SetAttribute("borderleft",info->m_type.m_nBorderLeft);
	screen->SetAttribute("bordertop",info->m_type.m_nBorderTop);
	screen->SetAttribute("borderright",info->m_type.m_nBorderRight);
	screen->SetAttribute("borderbottom",info->m_type.m_nBorderBottom);
	TiXmlElement position("Position");
	position.SetAttribute("centerx",info->m_posNew.m_Center.x);
	position.SetAttribute("centery",info->m_posNew.m_Center.y);
	position.SetAttribute("rotation",info->m_posNew.m_dRotation);
	position.SetAttribute("horizon",info->m_posNew.m_iHorizon);
	position.SetAttribute("vertical",info->m_posNew.m_iVertical);
	screen->InsertEndChild(position);
}
ExportInfo::~ExportInfo(void)
{
}

