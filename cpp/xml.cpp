#include "hpp/xml.h"

void Write2XML(TSInformation ts_information,vector<DetectionResult> detection)
{


	TiXmlDocument doc;
	TiXmlElement* msg;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement( "Commercial-Detection" );
	doc.LinkEndChild( root );

	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue("Copyright By FDU " );
	root->LinkEndChild( comment );

	TiXmlElement * msgs = new TiXmlElement( "Messages" );
	root->LinkEndChild( msgs );

	msg = new TiXmlElement( "Welcome" );
	msg->LinkEndChild( new TiXmlText( "Welcome to our Commercial-Detection system" ));
	msgs->LinkEndChild( msg );


	TiXmlElement * windows = new TiXmlElement( "TSInformation" );
	root->LinkEndChild( windows );

	TiXmlElement * window;
	window = new TiXmlElement( "Information" );
	windows->LinkEndChild( window );
	window->SetAttribute("ChannelName", ts_information.channel_name);
	window->SetAttribute("DateTime",ts_information.date);
	window->SetAttribute("Port",ts_information.port.c_str());
	window->SetAttribute("IPAddress",ts_information.ip_address.c_str());

	TiXmlElement * detection_result=new TiXmlElement("Detection-Result");
	root->LinkEndChild(detection_result);

	for (int i=0;i<detection.size();i++)
	{
		TiXmlElement * result = new TiXmlElement("Result");
		detection_result->LinkEndChild( result );
		result->SetAttribute("EndTime",detection.at(i).end_time);
		result->SetAttribute("StartTime",detection.at(i).start_time);
		result->SetAttribute("EndDTS",detection.at(i).end_dts);
		result->SetAttribute("StartDTS", detection.at(i).start_dts); // ¸¡µãÊýÊôÐÔ
		result->SetAttribute("CommercialID", detection.at(i).commercial_ID);

	}
	char * file_name=(char*)malloc(sizeof(char)*100);
	sprintf(file_name,"%s-%s%s",ts_information.channel_name,ts_information.date,".xml");
	doc.SaveFile( file_name );



}
