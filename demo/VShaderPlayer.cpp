//////////////////////////////////////////////////////////////////////

#include "VShaderPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// stworzenie obiektu i ustawienie wskaznikow

CVShaderPlayer::CVShaderPlayer()
{
	m_iSceneCount = 0;

	for (int i=0; i<128; i++) m_pVScene[i]=NULL;	// zerowanie wskaznika
	

}

// usuniecie obiektu i wyzerowanie wskaznikow

CVShaderPlayer::~CVShaderPlayer()
{

	// kasujemy wszystkie scenki

	for (int i=0; i<m_iSceneCount; i++)
	{
		delete m_pVScene[i];
	}

	m_iSceneCount = 0;

}

//=== parsowanie pliku

void CVShaderPlayer::Parse(char cFilename[])
{
	FILE	*plik;	// otwarty plik ze skryptem
	char	linia[1024];	// linia tekstu, ktora teraz parsujemy

	// sprawdz czy mozesz otworzyc plik

	if ((plik=fopen(cFilename,"r"))==NULL)
	{
		MessageBox(NULL,"ERROR! Brak pliku ze skryptami przy parsowaniu VShaderPlayer !","ERROR", MB_OK);
		PostQuitMessage( 0 );
	}

	// odczytujemy caly skrypt

	while (strcmp(linia,"</DEMO>"))
	{
		fscanf(plik,"%s\n",&linia);	// wczytaj linie pliku

		if (!strcmp(linia,"<V_SHADER>"))
		{
			fscanf(plik,"%s\n",&linia);	// data/background.jpg
			strcpy(m_cBackgroundFilename[m_iSceneCount],linia);

			fscanf(plik,"%s\n",&linia);	// data/obiekt.x
			strcpy(m_cSceneFilename[m_iSceneCount],linia);

			fscanf(plik,"%s\n",&linia);	// data/textura.tga
			strcpy(m_cTextureFilename[m_iSceneCount],linia);

			fscanf(plik,"%s\n",&linia);	// data/environment.jpg
			strcpy(m_cEnvironmentFilename[m_iSceneCount],linia);

			fscanf(plik,"%s\n",&linia);	// data/shader.vsh
			strcpy(m_cShaderFilename[m_iSceneCount],linia);
			
			fscanf(plik,"%d %d %d\n",&m_iStartTime[m_iSceneCount], &m_iEndTime[m_iSceneCount],
				&m_iLoadTime[m_iSceneCount]);	// data/environment.jpg

			fscanf(plik,"%d %d %d\n",&m_lStartX, &m_lStartY, &m_lStartZ);	// startowe wspolrzedne

			fscanf(plik,"%d %d %d\n",&m_lModifyX, &m_lModifyY, &m_lModifyZ);	// modifykatory

			// inicjalizacja nowej scenki, to nie jest jeszcze wczytanie danych

			m_pVScene[m_iSceneCount] = new CVShader();
			m_pVScene[m_iSceneCount]->SetStartPosition((float)m_lStartX,(float)m_lStartY,(float)m_lStartZ);
			m_pVScene[m_iSceneCount]->SetModificators((float)m_lModifyX,(float)m_lModifyY,(float)m_lModifyZ);			

			m_iSceneCount++;
		} // <VSHADER>
	}

}

//========================================================================================================================
// odtwarzanie odpowiedniej sceny w zaleznosci od licznika czasu
//========================================================================================================================

void CVShaderPlayer::Play(long lTimer, IDirect3DDevice8 *pDevice)
{

	// najpierw sprawdzamy czy jakas scenke nalezy zainicjalizowac

	for (int i=0; i<128; i++)
	{
		if (m_pVScene[i]!=NULL)
		{
			if (lTimer>=m_iLoadTime[i] && lTimer<m_iEndTime[i] && !m_pVScene[i]->bGetActive()) // sprawdzamy czy juz nie wczytane wczesniej
			{
				m_pVScene[i]->Initialize(pDevice,m_cBackgroundFilename[i],m_cSceneFilename[i],m_cTextureFilename[i],
					m_cEnvironmentFilename[i],m_cShaderFilename[i]);
			}
		}
	}

	// teraz sprawdzamy czy ktoras scenke nalezy odtworzyc

	for (int i=0; i<128; i++)
	{
		if (m_pVScene[i]!=NULL)
		{
			if (lTimer>=m_iStartTime[i] && lTimer<m_iEndTime[i])
			{
				m_pVScene[i]->DrawScene(pDevice,m_iStartTime[i],lTimer);
			}
		}
	}
	
	// teraz kasujemy te scenki, ktore juz nie sa potrzebne w pamieci, bo ich czas odtwarzania minal

	for (int i=0; i<128; i++)
	{
		if (m_pVScene[i]!=NULL && lTimer>m_iEndTime[i])
		{
			delete m_pVScene[i];
			m_pVScene[i]=NULL;
		}
	}

}

