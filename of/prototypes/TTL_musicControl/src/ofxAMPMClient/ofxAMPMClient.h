#pragma once

#include "ofxOsc.h"
#include "json.h" // eventually replace with ofJson

#include <queue>

namespace ofx {

typedef std::shared_ptr<class AMPMClient> AMPMClientRef;

class AMPMClient
{
		static AMPMClient* sInstance;

    ofxOscSender mSender;
    ofxOscReceiver mListener;

	public:
		static AMPMClient* get()
		{
			return sInstance;
		}
    
		enum LogEventLevel
		{
			info = 1,
			error,
			warning
		};

		static AMPMClientRef create( int sendPort, int recvPort );
		~AMPMClient();

        Json::Value getConfig();
		void update();
		void sendEvent( string category = "", string action = "", string label = "", int value = 0 );
		void sendCustomMessage( string address, Json::Value msg );
		void log( LogEventLevel level, string msg, char const* line, int lineNum );
		static char const* getFileForLog( char const* file );

	protected:
		AMPMClient( int sendPort, int recvPort );
		void sendHeartbeat();
};
};

// log macros (quick way to send log events to server)
#define LOG(M) AMPMClient::get()->log(AMPMClient::LogEventLevel::info, M, AMPMClient::getFileForLog(__FILE__), __LINE__)
#define LOG_ERR(M) AMPMClient::get()->log(AMPMClient::LogEventLevel::error, M, AMPMClient::getFileForLog(__FILE__), __LINE__)
#define LOG_WARN(M) AMPMClient::get()->log(AMPMClient::LogEventLevel::warning, M, AMPMClient::getFileForLog(__FILE__), __LINE__)
