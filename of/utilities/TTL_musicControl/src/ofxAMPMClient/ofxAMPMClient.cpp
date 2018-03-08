#include "ofxAMPMClient.h"

using namespace ofx;

string LogEventLevelToString( AMPMClient::LogEventLevel level ){
    switch (level) {
        case AMPMClient::LogEventLevel::info:
            return "info";
            break;
            
        case AMPMClient::LogEventLevel::warning:
            return "warning";
            break;
            
        case AMPMClient::LogEventLevel::error:
            return "error";
            break;
    }
};

AMPMClient* AMPMClient::sInstance = NULL;

AMPMClientRef AMPMClient::create( int sendPort, int recvPort )
{
	sInstance = new AMPMClient( sendPort, recvPort );
	return AMPMClientRef( sInstance );
}

AMPMClient::~AMPMClient()
{
}

AMPMClient::AMPMClient( int sendPort, int recvPort )
	: mSender(),
	  mListener()
{
    
	// setup osc
    mSender.setup( "127.0.0.1", sendPort ),
    mListener.setup( recvPort );
}

Json::Value AMPMClient::getConfig()
{
	Json::Value config;
    Json::Reader reader;

	try
	{
        ofURLFileLoader loader;
        ofHttpResponse resp = loader.get("http://localhost:8888/config");
//        if ( resp.status == 200)
        reader.parse(resp.data, config);
	}
    catch( exception ex )
	{
		LOG( ex.what() );
        ofLogError() << ( ex.what() );
	}

	return config;
}

void AMPMClient::update()
{
	// send heartbeat
	sendHeartbeat();
}

// send heartbeat to server
void AMPMClient::sendHeartbeat()
{
	ofxOscMessage message;
	message.setAddress( "/heart" );
	mSender.sendMessage( message );
}

// send analytics event to server
void AMPMClient::sendEvent( std::string category, std::string action, std::string label, int value )
{
    ofxOscMessage message;
	message.setAddress( "/event" );

    Json::FastWriter jWriter;
    Json::Value arguments(Json::objectValue);
    
    arguments["Category"] = category;
    arguments["Action"] = action;
    arguments["Label"] = label;
    arguments["Value"] = value;
    message.addStringArg( jWriter.write(arguments) );
    
    cout<<arguments.toStyledString()<<endl;
    
	mSender.sendMessage( message );
}

// send log event to server
void AMPMClient::log( LogEventLevel level, std::string msg, char const* line, int lineNum )
{
    ofxOscMessage message;
	message.setAddress( "/log" );

    Json::FastWriter jWriter;
    Json::Value arguments(Json::objectValue);
    
    arguments["level"] = LogEventLevelToString( level );
    arguments["message"] = msg;
    arguments["line"] = line;
    arguments["lineNum"] = lineNum;
    message.addStringArg( jWriter.write(arguments) );
    mSender.sendMessage( message );
}

// send custom osc message
void AMPMClient::sendCustomMessage( std::string address, Json::Value msg )
{
    Json::FastWriter jWriter;
    ofxOscMessage message;
	message.setAddress( address );
	message.addStringArg( jWriter.write(msg) );
	mSender.sendMessage( message );
}

// strip out file for sending as part of log info
char const* AMPMClient::getFileForLog( char const* file )
{
	return strrchr( file, '\\' ) ? strrchr( file, '\\' ) + 1 : file;
}
