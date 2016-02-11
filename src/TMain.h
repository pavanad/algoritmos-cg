//---------------------------------------------------------------------------

#ifndef TMainH
#define TMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <vector>
//---------------------------------------------------------------------------

#define Vertice	std::vector< std::pair<int,int> >

/**
 * Armazena a estrutura das retas criadas
 */
struct LineSegment
{
	double a,b;
	int xi, yi, xf, yf;
};

class TMain : public TForm
{
__published:

	TToolBar *m_tools;
	TStatusBar *m_status;
	TToolButton *m_ToolLine;
	TImageList *ImageList1;
	TToolButton *m_ToolIntersec;
	TPaintBox *m_Paint;
	TToolButton *m_ToolPoly;
	TToolButton *m_ToolCircle;
	TPanel *m_PanelPaint;
	TPanel *m_PanelLog;
	TSplitter *m_splitterLog;
	TToolButton *m_ToolErase;
	TMainMenu *MainMenu1;
	TMenuItem *Arquivo1;
	TMenuItem *Reta1;
	TMenuItem *Verificarinterseces1;
	TMenuItem *N1;
	TMenuItem *Limparmensagens1;
	TMenuItem *Exibir1;
	TMenuItem *Barradeferramentas1;
	TMenuItem *Desenharcircunferncia1;
	TMenuItem *Limpardesenhos1;
	TMenuItem *N2;
	TMenuItem *Sair1;
	TMenuItem *Barradestatus1;
	TActionList *ActionList1;
	TAction *m_actionClearDraws;
	TAction *m_actionIntersec;
	TAction *m_actionLine;
	TAction *m_actionClearMsg;
	TAction *m_actionCircle;
	TToolButton *ToolButton1;
	TToolButton *m_ToolLineFull;
	TPopupMenu *PopupMenu1;
	TMenuItem *Desenharreta1;
	TMenuItem *Desenharcircunferncia2;
	TMenuItem *Verificarinterseces2;
	TMenuItem *N3;
	TMenuItem *Limpardesenhos2;
	TMenuItem *Limparmensagens2;
	TMenuItem *N4;
	TMenuItem *Exibir2;
	TMenuItem *Barradestatus2;
	TMenuItem *Barradeferramentas2;
	TAction *m_actionStatus;
	TAction *m_actionTools;
	TAction *m_actionPoly;
	TMenuItem *Desenharpolgono1;
	TMenuItem *Desenharpolgono2;
	TRichEdit *m_log;
	void __fastcall m_PaintMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall m_PaintClick(TObject *Sender);
	void __fastcall Sair1Click(TObject *Sender);
	void __fastcall m_PaintPaint(TObject *Sender);
	void __fastcall m_actionClearDrawsExecute(TObject *Sender);
	void __fastcall m_actionIntersecExecute(TObject *Sender);
	void __fastcall m_actionLineExecute(TObject *Sender);
	void __fastcall m_actionClearMsgExecute(TObject *Sender);
	void __fastcall m_actionCircleExecute(TObject *Sender);
	void __fastcall m_actionStatusExecute(TObject *Sender);
	void __fastcall m_actionToolsExecute(TObject *Sender);
	void __fastcall m_actionPolyExecute(TObject *Sender);
	void __fastcall m_PaintDblClick(TObject *Sender);

private:

	TPoint m_point;				//armazena o ponto atual das coordenadas do mouse

	bool m_checkLine;			//flag de utilização da ferramenta reta
	bool m_checkCircle;		//flag de utilização da ferramenta circunferência
	bool m_checkPoly;			//flag de utilização da ferramenta de poligono

	std::vector< std::pair<int,int> > m_vecPoint;				//vetor armazena os pontos
	std::vector< std::pair<int,int> > m_vecPointPoly;		//vetor armazena os pontos do poligono

	//vetor armazena os coeficientes das retas
	std::vector<LineSegment> m_vecLine;

	//vetor armazena as vertices dos poligonos
	std::vector<Vertice> m_VerticePoly;

	/**
	 * Calcula o coeficiente angular dos pontos
	 *
	 * @param	xi	Recebe o valor de x do ponto inicial
	 * @param	yi	Recebe o valor de y do ponto inicial
	 * @param	xf	Recebe o valor de x do ponto final
	 * @param	yf	Recebe o valor de y do ponto final
	 * @return		Retorna um double com o coeficiente angular
	 */
	double GetSlope(int xi, int yi, int xf, int yf);

	/**
	 * Cálcula o coeficiente linear dos pontos
	 *
	 * @param	x				Valor de x das coordenadas do ponto
	 * @param	y				Valor de y das coordenadas do ponto
	 * @param slope		Valor do coeficiente angular
	 * @return				Retorna um double com o coeficiente linear
	 */
	double GetLinearCoeficient(int x, int y, double slope);

	/**
	 * Método cálcula os coeficientes angular, linear e
	 * desenha a reta no componente TPaintBox
	 */
	void DrawLineTo();

	/**
	 * Desenha os pontos até formar a reta
	 *
	 * @param	xi			Recebe o valor de x do ponto inicial
	 * @param	yi			Recebe o valor de y do ponto inicial
	 * @param	xf			Recebe o valor de x do ponto final
	 * @param	yf			Recebe o valor de y do ponto final
	 * @param slope		Valor do coeficiente angular
	 * @param	linear	Valor do coeficiente linear
	 */
	void DrawLine(int xi, int yi, int xf, int yf, double slope, double linear);

  /**
	 * Calcula o raio da circunferência
	 *
	 * @param	xi			Recebe o valor de x do ponto inicial
	 * @param	yi			Recebe o valor de y do ponto inicial
	 * @param	xf			Recebe o valor de x do ponto final
	 * @param	yf			Recebe o valor de y do ponto final
	 * @return				Retorna o raio da circunferência
	 */
	double GetRaio(int xi, int yi, int xf, int yf);

	/**
	 * Cálcula o limite que o eixo x deve percorrer
	 *
	 * @param	r	Recebe o raio da circunferência
	 * @return	Retorna o limite
	 */
	int GetLimit(double r);

	/**
	 * Método cálcula o raio, o limite e
	 * desenha a circunferência
	 */
	void DrawCircle();

	/**
	 * Método limpa o componente TPaintBox
	 */
	void ClearPaint();

	/**
	 * Método faz o tratamente do ponto de intersecção apenas no
	 * segmento de reta
	 *
	 * @param	xit			Recebe o valor de x do ponto de intersecção
	 * @param	yit			Recebe o valor de y do ponto de intersecção
	 * @param	xi			Recebe o valor de x do ponto inicial
	 * @param	yi			Recebe o valor de y do ponto inicial
	 * @param	xf			Recebe o valor de x do ponto final
	 * @param	yf			Recebe o valor de y do ponto final
	 * @return				Retorna true se o ponto está no segmento de reta
	 */
	bool CheckLineSegment(int xit, int yit, int xi, int yi, int xf, int yf);

	/**
	 * Método desenha o poligono
	 */
	void DrawPolygon();

	/**
	 * Método localiza o token no log e destaca
	 *
	 * @param	token		String com o token a ser localilzado
	 */
	void ColorLog(UnicodeString token);

public:

	__fastcall TMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
