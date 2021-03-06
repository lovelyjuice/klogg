#include <QFile>

#include "file_write_helper.h"
#include <log.h>
#include <plog/Appenders/ConsoleAppender.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // _WIN32

static const char* sl_format = "LOGDATA is a part of glogg, we are going to test it thoroughly, this is line %06d\n";

int main( int argc, const char** argv )
{
	plog::ConsoleAppender<plog::GloggFormatter> appender;
	plog::init(logINFO, &appender);

    if ( argc < 4) {
        LOG(logERROR) << "Expected 3 arguments";
    }

	LOG(logINFO) << "Will write to " << argv[1] << " lines " << argv[2] << ", flag " << argv[3];

	QFile file{ argv[1] };

	file.open( QIODevice::Unbuffered | QIODevice::WriteOnly | QIODevice::Append );

	if ( !file.isOpen() ) {
		return -1;
	}
	
	const int numberOfLines = atoi( argv[2] );
	const auto flag = static_cast<WriteFileModification>( atoi( argv[3] ) );

	if ( flag == WriteFileModification::StartWithPartialLineEnd )
	{
		file.write( partial_line_end, qstrlen( partial_line_end ) );
	}

	char newLine[90];
	for ( int i = 0; i < numberOfLines; i++ ) {
		snprintf( newLine, 89, sl_format, i );
		file.write( newLine, qstrlen( newLine ) );
	}

	if ( flag == WriteFileModification::EndWithPartialLineBegin )
	{
		file.write( partial_line_begin, qstrlen( partial_line_begin ) );
	}

#ifdef _WIN32
	FlushFileBuffers(reinterpret_cast<HANDLE>(_get_osfhandle(file.handle())));
#endif // _WIN32

	file.close();

	file.open(QIODevice::Unbuffered | QIODevice::ReadOnly | QIODevice::Append);

	LOG(logINFO) << "Write to " << argv[1] << " finished, size " << file.size();

	file.close();

	return 0;
}
