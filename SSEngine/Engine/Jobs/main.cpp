#include <Windows.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG

#include <conio.h>

#include "SubSystem\Job\JobSystem.h"
#include "Jobs\IJob.h"

//class PrintOnInterval : public IJob
//{
//public:
//	PrintOnInterval( const char * i_pString, unsigned int i_IntervalMilliseconds, unsigned int i_Count = 0 ) :
//		IJob( "Print On Interval" ),
//		m_String( i_pString ? i_pString : "Error" ),
//		m_Interval( i_IntervalMilliseconds ),
//		m_Count( i_Count )
//	{
//		assert( i_pString );
//		assert( m_Interval > 0 );
//	}
//
//	void Run()
//	{
//		bool bCounted = m_Count > 0;
//		
//		do
//		{
//			printf( "%s\n", m_String.c_str() );
//			Sleep( m_Interval );
//		} while( (!bCounted  ||  (bCounted  && --m_Count)) && !ShutdownRequested() );
//	}
//
//private:
//	std::string		m_String;
//	unsigned int	m_Interval;
//	unsigned int	m_Count;
//};
//
//class ProcessFile_Process : public Engine::JobSystem::IJob
//{
//
//public:
//	ProcessFile_Process( uint8_t * i_pFileContents, size_t i_sizeFileContents ) :
//		IJob( "ProcessFile_Process" ),
//		m_pFileContents( i_pFileContents ),
//		m_sizeFileContents( i_sizeFileContents )
//	{}
//
//	void Run()
//	{
//		if( m_sizeFileContents  && m_pFileContents )
//		{
//			char * pText = new char[m_sizeFileContents + 1];
//			memcpy( pText, m_pFileContents, m_sizeFileContents );
//			pText[m_sizeFileContents] = 0;
//
//			puts( pText );
//
//			delete[] pText;
//			delete[] m_pFileContents;
//		}
//	}
//private:
//	uint8_t *		m_pFileContents;
//	size_t			m_sizeFileContents;
//};
//
//class ProcessFile : public Engine::JobSystem::IJob
//{
//public:
//	ProcessFile( const char * i_pFilename ) :
//		IJob( "ProcessFile" ),
//		m_pFilename( i_pFilename )
//	{
//		assert( m_pFilename );
//	}
//	void Run()
//	{
//
//		if( m_pFilename )
//		{
//			size_t sizeFileContents = 0;
//			uint8_t * pFileContents = LoadFile( m_pFilename, sizeFileContents );
//
//			if( pFileContents && sizeFileContents )
//			{
//				IJob * pProcessJob = new ProcessFile_Process( pFileContents, sizeFileContents );
//				assert( pProcessJob );
//
//				Engine::JobSystem::RunJob( *pProcessJob, "Default" );
//			}
//		}
//	}
//
//	static uint8_t * LoadFile( const char * i_pFilename, size_t & o_sizeFile )
//	{
//		assert( i_pFilename != NULL );
//
//		FILE *				pFile = NULL;
//
//		errno_t fopenError = fopen_s( &pFile, i_pFilename, "rb" );
//		if( fopenError != 0 )
//			return NULL;
//
//		assert( pFile != NULL );
//
//		int FileIOError = fseek( pFile, 0, SEEK_END );
//		assert( FileIOError == 0 );
//
//		long FileSize = ftell( pFile );
//		assert( FileSize >= 0 );
//
//		FileIOError = fseek( pFile, 0, SEEK_SET );
//		assert( FileIOError == 0 );
//
//		uint8_t * pBuffer = new uint8_t[FileSize];
//		assert( pBuffer );
//
//		size_t FileRead = fread( pBuffer, 1, FileSize, pFile );
//		assert( FileRead == FileSize );
//
//		fclose( pFile );
//
//		o_sizeFile = FileSize;
//
//		return pBuffer;
//	}
//private:
//	const char *		m_pFilename;
//};
//
//int main( int i_argc, char ** i_argl )
//{
//#if defined _DEBUG
//	// _CrtSetBreakAlloc( 156 );
//#endif // _DEBUG
//
//	{
//		Engine::JobSystem::CreateQueue( "Default", 2 );
//
//		PrintOnInterval * pSayHi = new PrintOnInterval( "Hi!", 1000, 0 );
//		assert( pSayHi );
//		Engine::JobSystem::RunJob( *pSayHi, "Default" );
//
//		ProcessFile * pProcessText = new ProcessFile( "File.txt" );
//		assert( pProcessText );
//		Engine::JobSystem::RunJob( *pProcessText, "Default" );
//
//		PrintOnInterval	* pSayWahoo = new PrintOnInterval( "Wahoo!!", 500, 1 );
//		assert( pSayWahoo );
//		Engine::JobSystem::RunJob( *pSayWahoo, "Default" );
//
//		_getch();
//
//		Engine::JobSystem::Shutdown();
//	}
//
//#if defined _DEBUG
//	_CrtDumpMemoryLeaks();
//#endif // _DEBUG
//	return 0;
//}
