//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMain *Main;
//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
	: TForm(Owner)
{
	m_checkLine = false;
	m_checkCircle = false;
	m_checkPoly = false;
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_PaintMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	//armazena os pontos do mouse
	m_point.x = X;
	m_point.y = m_Paint->Height - Y;

	//imprime as coordenadas na barra de status
	UnicodeString coord = " X: " + IntToStr(X) + " - Y: " + IntToStr(m_Paint->Height-Y);
	m_status->Panels->Items[0]->Text = coord;
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_PaintClick(TObject *Sender)
{
	if(m_checkLine)	//verifica se a ferramenta reta foi selecionada
	{
		//imprime os pontos iniciais
		m_Paint->Canvas->Pixels[m_point.x][m_Paint->Height - m_point.y] = clBlack;

		//desenha a reta
		this->DrawLineTo();
	}

	if (m_checkCircle) //verifica se a ferramenta circular foi selecionada
	{
  	//imprime os pontos iniciais
		m_Paint->Canvas->Pixels[m_point.x][m_Paint->Height - m_point.y] = clBlack;

    this->DrawCircle();
	}

	if (m_checkPoly)	//verifica se a ferramenta poligono foi selecionada
	{
		//imprime os pontos iniciais
		m_Paint->Canvas->Pixels[m_point.x][m_Paint->Height - m_point.y] = clBlack;

    this->DrawPolygon();
	}
}
//---------------------------------------------------------------------------
double TMain::GetSlope(int xi, int yi, int xf, int yf)
{
	return (double)(yf - yi) / (xf - xi);
}
//---------------------------------------------------------------------------
double TMain::GetLinearCoeficient(int x, int y, double slope)
{
	return (double) y - (slope * x);
}
//---------------------------------------------------------------------------
void TMain::DrawLine(int xi, int yi, int xf, int yf, double slope, double linear)
{
	int posI, posF;

	if (slope < 1) //coeficiente angular < 1 realiza varredura por X
	{
		if(xf < xi)
		{
			posI = xf+1;
			posF = xi-1;
		}
		else
		{
			posI = xi+1;
			posF = xf-1;
		}
		m_log->Lines->Add("Log >> Varredura pelo eixo X\n");
	}
	else	//coeficiente angular > 1 realiza varredura por Y
	{
		if(yf < yi)
		{
			posI = yf+1;
			posF = yi-1;
		}
		else
		{
			posI = yi+1;
			posF = yf-1;
		}
		m_log->Lines->Add("Log >> Varredura pelo eixo Y\n");
	}

  int y = 0, x = 0;

	for (int i = posI; i <= posF; i++)
	{
		if (slope < 1)
		{
			//equação completa da reta (varredura por x)

			y = (slope*i) + linear;
			m_Paint->Canvas->Pixels[i][m_Paint->Height - y] = clBlack;
		}
		else
		{
			//equação completa da reta (varredura por y)

			x = (i - linear)/slope;
			m_Paint->Canvas->Pixels[x][m_Paint->Height - i] = clBlack;
		}
	}
}
//---------------------------------------------------------------------------
void TMain::DrawCircle()
{

	if(m_vecPoint.size() < 2)
	{
		m_vecPoint.push_back(std::make_pair(m_point.x,m_point.y));

		if(m_vecPoint.size()==2)
		{
			//calcula o raio
			double raio = this->GetRaio(m_vecPoint[0].first,m_vecPoint[0].second,
																	m_vecPoint[1].first,m_vecPoint[1].second);

			//calcula o limite
			int limit = this->GetLimit(raio) + 1;

			m_log->SelAttributes->Color = clBlue;
			m_log->Lines->Add("Criação da circunferência");
			m_log->Lines->Add("Log >> Raio: " + FloatToStr(raio));
			m_log->Lines->Add("Log >> Limite: " + IntToStr(limit) + "\n");

			for (int x = 0; x <= limit; x++)
			{
				double aux = (raio*raio) - (x*x);
				int y = Sqrt(aux);

				/*
				 * Faz o calculo das posições dos octantes:
				 * como o plano cartesiano é invertido e não existem números negativos
				 * apenas a mudança de sinal não é suficiente para realizar a simetria,
				 * sendo assim foi necessário adaptar os calculos para a posição correta
				 */

				int xx = m_vecPoint[0].first + x;
				int invertX = m_vecPoint[0].first - x; //adaptação para o x negativo

				//adaptação do calculo para alimentar o eixo Y com o valor de X
				int rangeX = m_Paint->Height - (m_vecPoint[0].second + x);
				int invRangeX = m_Paint->Height - (m_vecPoint[0].second - x);

				int yy = m_Paint->Height - (y + m_vecPoint[0].second);
				int invertY = m_Paint->Height - (m_vecPoint[0].second - y); //y negativo

				//adaptação do calculo para alimentar o eixo x com o valor de y
				int rangeY = m_vecPoint[0].first + y;
				int invRangeY = m_vecPoint[0].first - y;

				//preenche todos os octantes
				m_Paint->Canvas->Pixels[xx][yy] = clBlack;
				m_Paint->Canvas->Pixels[xx][invertY] = clBlack;
				m_Paint->Canvas->Pixels[invertX][yy] = clBlack;
				m_Paint->Canvas->Pixels[invertX][invertY] = clBlack;

				//preenchimento com a inversão dos valores
				m_Paint->Canvas->Pixels[rangeY][rangeX] = clBlack;
				m_Paint->Canvas->Pixels[invRangeY][rangeX] = clBlack;
				m_Paint->Canvas->Pixels[rangeY][invRangeX] = clBlack;
				m_Paint->Canvas->Pixels[invRangeY][invRangeX] = clBlack;
			}
			m_vecPoint.clear();

			this->ColorLog("Log >> ");
		}
	}

}
//---------------------------------------------------------------------------
void TMain::DrawLineTo()
{
	if(m_vecPoint.size() < 2)
	{
		m_vecPoint.push_back(std::make_pair(m_point.x,m_point.y));

		if(m_vecPoint.size()==2)
		{
			//calcula o coeficiente angular
			double a = this->GetSlope(m_vecPoint[0].first,m_vecPoint[0].second,
																m_vecPoint[1].first,m_vecPoint[1].second);


			//calcula o coeficiente linear
			double b = this->GetLinearCoeficient(m_vecPoint[0].first,m_vecPoint[0].second,a);

			m_log->SelAttributes->Color = clBlue;
			m_log->Lines->Add("Criação da reta");
			UnicodeString msgPoint = "Log >> Ponto inicial: X:" + IntToStr(m_vecPoint[0].first) +
															 ", Y:" + IntToStr(m_vecPoint[0].second) + " - " +
																									 "Ponto final:  X:" + IntToStr(m_vecPoint[1].first) +
															 ", Y:" + IntToStr(m_vecPoint[1].second);

			//escreve as coordenadas no log
			m_log->Lines->Add(msgPoint);
			m_log->SelAttributes->Color = clMaroon;
			m_log->Lines->Add("Log >> Cálculo dos coeficientes");
			m_log->Lines->Add("Log >> Coeficiente angular: " + FloatToStr(a));
			m_log->Lines->Add("Log >> Coeficiente linear:    " + FloatToStr(b));

			//desenha a reta
			this->DrawLine(m_vecPoint[0].first,m_vecPoint[0].second,
										 m_vecPoint[1].first,m_vecPoint[1].second, a, b);

			//armazena os coeficientes angulares, lineares, xi e xf
			LineSegment line;
			line.xi = m_vecPoint[0].first;
			line.yi = m_vecPoint[0].second;
			line.xf = m_vecPoint[1].first;
			line.yf = m_vecPoint[1].second;
			line.a = a;
			line.b = b;

			m_vecLine.push_back(line);
			m_vecPoint.clear();

			this->ColorLog("Log >> ");
		}
	}
}
//---------------------------------------------------------------------------
void TMain::ClearPaint()
{
	m_Paint->Canvas->Brush->Color = clWhite;
	m_Paint->Canvas->Brush->Style = bsSolid;

	m_Paint->Canvas->FillRect(m_Paint->ClientRect);
}
//---------------------------------------------------------------------------
bool TMain::CheckLineSegment(int xit, int yit, int xi, int yi, int xf, int yf)
{
	int aux = 0;

	if (xf < xi)	//inverte os eixos caso o final seja menor
	{
		aux = xf;
		xf = xi;
		xi = aux;
	}

	if (yf < yi) //inverte os eixos caso o final seja menor
	{
		aux = yf;
		yf = yi;
		yi = aux;
	}

	//compara se o ponto está apenas no segmento de reta

	if (xit < xi || xit > xf || yit < yi || yit > yf)
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TMain::Sair1Click(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_PaintPaint(TObject *Sender)
{
	this->ClearPaint();
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionClearDrawsExecute(TObject *Sender)
{
	int dlg = MessageDlg("Deseja realmente limpar ?",mtConfirmation,mbYesNo,0);
	if (dlg==6)
	{
		this->ClearPaint();
		m_log->Clear();
		m_vecLine.clear();
		m_vecPoint.clear();
		m_vecPointPoly.clear();
		m_VerticePoly.clear();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionIntersecExecute(TObject *Sender)
{
	if(m_vecLine.size() == 0)
	{
		MessageBox(NULL,"Nenhuma reta localizada","AlgoritmosCG", MB_OK | MB_ICONINFORMATION);
		return;
	}

	for (unsigned int i = 0; i < m_vecLine.size(); i++)
	{
		double a1 = m_vecLine[i].a;
		double b1 = m_vecLine[i].b;

		for (unsigned int j = i+1; j < m_vecLine.size(); j++)
		{
			double a2 = m_vecLine[j].a;
			double b2 = m_vecLine[j].b;

			if (a1 != a2) //verifica se os coef. angulares são diferentes
			{
      	m_log->SelAttributes->Color = clBlue;
				m_log->Lines->Add("Verificação dos pontos de intersecção");

				//localiza a coordenadas comparando as equações da reta
				int x = (int) (b2 - b1) / (a1 - a2);
				int y = (int)(a1 * x) + b1;

				/**
				 * Caso opção selecionada faz a verificação da equação
				 * completa da reta ou apenas do segmento de reta
				 */
				if(!m_ToolLineFull->Down)
				{
					//faz a verificação apenas no segmento da reta
					bool checkLine1 = this->CheckLineSegment(x,y,m_vecLine[i].xi,m_vecLine[i].yi,
																									 m_vecLine[i].xf,m_vecLine[i].yf);

					bool checkLine2 = this->CheckLineSegment(x,y,m_vecLine[i].xi,m_vecLine[j].yi,
																									 m_vecLine[i].xf,m_vecLine[j].yf);

					if(!checkLine1 || !checkLine2) continue;

					m_log->Lines->Add("Log >> verificação do segmento de reta");
				}
				else
				{
					m_log->Lines->Add("Log >> verificação da equação da reta completa");
        }

				//aumenta o tamanho do pixel
				for (int c = -1; c <= 2; c++)
				{
					for (int r = -3; r <= 2; r++)
					{
						m_Paint->Canvas->Pixels[x+c][m_Paint->Height - y + r] = clRed;
					}
				}

				//imprime a coordenadas
				UnicodeString pointInter = "PIT(" + IntToStr(x) + ", " +
																		IntToStr(m_Paint->Height - y) + ")";

				m_Paint->Canvas->Font->Color = clRed;
				m_Paint->Canvas->TextOut(x + 10, m_Paint->Height - y - 3, pointInter);

				m_log->Lines->Add("Log >> Pontos de intersecção: X:" + IntToStr(x) +
													"Y: " + IntToStr(m_Paint->Height - y) + "\n");

				this->ColorLog("Log >> ");
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionLineExecute(TObject *Sender)
{
	Reta1->Checked = !Reta1->Checked;
	Desenharreta1->Checked = !Desenharreta1->Checked;

	m_ToolLine->Down = Reta1->Checked;

	m_checkLine = !m_checkLine;
	if(m_checkLine)
	{
  	m_checkPoly = false;
		m_checkCircle = false;
	}

	if(Desenharcircunferncia1->Checked) Desenharcircunferncia1->Checked = false;
	if(Desenharcircunferncia2->Checked) Desenharcircunferncia2->Checked = false;
	if(m_ToolCircle->Down) m_ToolCircle->Down = false;

	if(Desenharpolgono1->Checked) Desenharpolgono1->Checked = false;
	if(Desenharpolgono2->Checked) Desenharpolgono2->Checked = false;
	if(m_ToolPoly->Down) m_ToolPoly->Down = false;
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionClearMsgExecute(TObject *Sender)
{
	int dlg = MessageDlg("Deseja realmente limpar as mensagens ?",mtConfirmation,mbYesNo,0);
	if (dlg==6)
	{
  	m_log->Clear();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionCircleExecute(TObject *Sender)
{
	Desenharcircunferncia1->Checked = !Desenharcircunferncia1->Checked;
	Desenharcircunferncia2->Checked = !Desenharcircunferncia2->Checked;
	m_ToolCircle->Down = Desenharcircunferncia1->Checked;

	m_checkCircle = !m_checkCircle;
	if(m_checkCircle)
	{
  	m_checkPoly = false;
		m_checkLine = false;
	}

	if(Reta1->Checked) Reta1->Checked = false;
	if(Desenharreta1->Checked) Desenharreta1->Checked = false;
	if(m_ToolLine->Down)m_ToolLine->Down = false;

	if(Desenharpolgono1->Checked) Desenharpolgono1->Checked = false;
	if(Desenharpolgono2->Checked) Desenharpolgono2->Checked = false;
	if(m_ToolPoly->Down) m_ToolPoly->Down = false;
}
//---------------------------------------------------------------------------
double TMain::GetRaio(int xi, int yi, int xf, int yf)
{
	int x = xf-xi;
	int y = yf-yi;
	double aux = (y*y) + (x*x);

	return Sqrt(aux);
}
//---------------------------------------------------------------------------
int TMain::GetLimit(double r)
{
	const double cos45 = 0.70710678118654752440084436210485;

	return cos45 * r;
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionStatusExecute(TObject *Sender)
{
	m_status->Visible = Barradestatus2->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionToolsExecute(TObject *Sender)
{
	m_tools->Visible = Barradeferramentas2->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_actionPolyExecute(TObject *Sender)
{
	Desenharpolgono1->Checked = !Desenharpolgono1->Checked;
	Desenharpolgono2->Checked = !Desenharpolgono2->Checked;
	m_ToolPoly->Down = Desenharpolgono1->Checked;

	m_checkPoly = !m_checkPoly;

	if (m_checkPoly)
	{
		m_checkLine = false;
		m_checkCircle = false;
	}

	if(Reta1->Checked) Reta1->Checked = false;
	if(Desenharreta1->Checked) Desenharreta1->Checked = false;
	if(m_ToolLine->Down)m_ToolLine->Down = false;

	if(Desenharcircunferncia1->Checked) Desenharcircunferncia1->Checked = false;
	if(Desenharcircunferncia2->Checked) Desenharcircunferncia2->Checked = false;
	if(m_ToolCircle->Down) m_ToolCircle->Down = false;
}
//---------------------------------------------------------------------------
void TMain::DrawPolygon()
{
	m_vecPointPoly.push_back(std::make_pair(m_point.x,m_point.y));

	if(m_vecPointPoly.size()>=2)
	{
		for (unsigned int i = 0; i < m_vecPointPoly.size(); i++)
		{
			if(i+1 < m_vecPointPoly.size())
			{
				//calcula o coeficiente angular
				double a = this->GetSlope(m_vecPointPoly[i].first,m_vecPointPoly[i].second,
																	m_vecPointPoly[i+1].first,m_vecPointPoly[i+1].second);

				//calcula o coeficiente linear
				double b = this->GetLinearCoeficient(m_vecPointPoly[i].first,m_vecPointPoly[i].second,a);

				//desenha a reta
				this->DrawLine(m_vecPointPoly[i].first,m_vecPointPoly[i].second,
											 m_vecPointPoly[i+1].first,m_vecPointPoly[i+1].second, a, b);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::m_PaintDblClick(TObject *Sender)
{
	/**
	 * Finaliza o desenho do poligono ligando o ultimo ponto
	 * armazenado com o primeiro
	 */

	if (m_checkPoly)	//verifica se a ferramenta poligono foi selecionada
	{
		if(m_vecPointPoly.size() > 2)
		{
			int last = m_vecPointPoly.size() - 1; //ultimo ponto armazenado

			//calcula o coeficiente angular
			double a = this->GetSlope(m_vecPointPoly[last].first,m_vecPointPoly[last].second,
																m_vecPointPoly[0].first,m_vecPointPoly[0].second);

			//calcula o coeficiente linear
			double b = this->GetLinearCoeficient(m_vecPointPoly[last].first,m_vecPointPoly[last].second,a);

			//desenha a reta
			this->DrawLine(m_vecPointPoly[last].first,m_vecPointPoly[last].second,
										 m_vecPointPoly[0].first,m_vecPointPoly[0].second, a, b);

			m_VerticePoly.push_back(m_vecPointPoly);
			m_vecPointPoly.clear();
		}
	}
}
//---------------------------------------------------------------------------
void TMain::ColorLog(UnicodeString token)
{
	int pos = 0;
	int foundAt = m_log->FindText(token,pos,m_log->Text.Length(),TSearchTypes());

	while(foundAt!=-1)
	{
		m_log->SelStart = foundAt;
		m_log->SelLength = token.Length();
		m_log->SelAttributes->Color = (TColor)0x00FF0080;

		pos = foundAt + token.Length();
		foundAt = m_log->FindText(token,pos,m_log->Text.Length(),TSearchTypes());
	}
	m_log->SelStart = m_log->Text.Length();
}
//---------------------------------------------------------------------------
