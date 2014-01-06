//Clop Debug output.
static ClopHandler* clopHandlerDebug;
static ClopHandler* clopHandlerFatal;
static ClopHandler* clopHandlerError;
static ClopHandler* clopHandlerWarning;

void clopLoggerCallback( LogSystem::LogType m_type, const char * message )
{
    switch( m_type )
    {
    case LogSystem::LogType::logType_debug :
        Core::Console().PrintLine(std::string( message ), Colors::White );
        break;

    case LogSystem::LogType::logType_error :
        Core::Console().PrintLine(std::string( message ), Colors::Red );
        break;

    case LogSystem::LogType::logType_fatal :
        Core::Console().PrintLine(std::string( message ), Colors::Red );
        break;

    case LogSystem::LogType::logType_warning :
        Core::Console().PrintLine(std::string( message ), Colors::Yellow);
        break;
    }
}

static void RegisterCLOPLogger()
{
    clopHandlerDebug = new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_debug );
    clopHandlerFatal = new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_fatal );
    clopHandlerError = new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_error );
    clopHandlerWarning  = new ClopHandler( clopLoggerCallback, LogSystem::LogType::logType_warning);

    LogSystem::RegisterLogHandler( LogSystem::debugHandler, clopHandlerDebug );
    LogSystem::RegisterLogHandler( LogSystem::fatalHandler, clopHandlerFatal );
    LogSystem::RegisterLogHandler( LogSystem::errorHandler, clopHandlerError );
    LogSystem::RegisterLogHandler( LogSystem::warningHandler, clopHandlerWarning );
}

static void DeregisterCLOPLogger()
{
    LogSystem::DeregisterLogHandler( LogSystem::debugHandler, clopHandlerDebug );
    LogSystem::DeregisterLogHandler( LogSystem::fatalHandler, clopHandlerFatal );
    LogSystem::DeregisterLogHandler( LogSystem::errorHandler, clopHandlerError );
    LogSystem::DeregisterLogHandler( LogSystem::warningHandler, clopHandlerWarning );

    delete clopHandlerDebug;
    delete clopHandlerFatal;
    delete clopHandlerError;
    delete clopHandlerWarning;
}
