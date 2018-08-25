#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>






#define MAX_BUFFER_SIZE 4096  // a voir si pas assez grand pour un reccord

#include <fstream>
#include<string>
#include <iostream>

#include <tinyxml2.h>
using namespace tinyxml2;
using namespace std;

///$$$$$$$$$$$$$ string fixed $$$$$$$$$$$$$$$$$$$$$


///$$$$$$$$$$$$$ XML $$$$$$$$$$$$$$$$$$$$$

void build_entete_doc(XMLDocument * doc,   XMLElement  * corpx , string elem, string texte );
void build_field_doc(XMLElement  * atr  , string elem, string texte );
bool read_DSPF(string filexml);
string write_DSPF();

string RCDFMT ;

string DATE;
string NOM;
string PRENOM;
string NRUE;
string RUE1;
string RUE2;


//void rcv_xml(string vXML);

///$$$$$$$$$ SERVEUR $$$$$$$$$$$$$$$
struct lws_context * context;


string WEB_MESSAGE ="";
string XMLBuffer;
string line;
bool ok = false ;

bool msg_SIGTERM = false ;
FILE *flog ;





struct per_session_data__client {
    char client_name [256];
	char client_ip[45];
	char client_njob[27];
	int Num_Client;
	int number =0;
};

struct connect_web {;
    char client_name [256];
    char client_ip[45]; // prev The correct maximum IPv6 string length is 45 characters
} webcon;


int Client_web = 0;
struct per_session_data__client *pss ;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

static int callback_main( struct lws *wsi,
                            enum lws_callback_reasons reason,
                            void *user,
                            void *in,
                            size_t len )


{


    char *buf = (char*) malloc(LWS_PRE +MAX_BUFFER_SIZE);

    	pss = (struct per_session_data__client*) user;


    switch ((int)reason) {


    case  LWS_CALLBACK_ESTABLISHED: {
            lwsl_notice("connection established\n  N° client : %d\n" , Client_web );

            sprintf(pss->client_name,"%s",webcon.client_name);
            sprintf(pss->client_ip,"%s",webcon.client_ip);
            pss->number =Client_web;
            pss->Num_Client = 79928; // pour le moment  récupéré avec bd

            sprintf(pss->client_njob,"%s","20150609195701433872653505"); // calcul avec date ect...
            lwsl_notice("N° connection  %d \n  IP : %s  Name: %s\n njob:%s\n" , Client_web, pss->client_ip, pss->client_name,pss->client_njob );

            lwsl_notice("pss->number :%d\n",pss->number);

            WEB_MESSAGE="Vous êtes connecté au  SERVEUR";
            lws_callback_on_writable(wsi);
            break;
    }
    case LWS_CALLBACK_SERVER_WRITEABLE: {
            memset(&buf[LWS_PRE], 0, MAX_BUFFER_SIZE);

            if(WEB_MESSAGE >"") {

                            /// preparation de la sortie bien respecter le tampon  attention ça peux vite devenir un casse tête
                            /// norme https://libwebsockets.org/lws-api-doc-master/html/index.html
                               strcpy(buf+ LWS_PRE,WEB_MESSAGE.c_str() );
                               lws_write(wsi, (unsigned char *) &buf[LWS_PRE], strlen(&buf[LWS_PRE]), LWS_WRITE_TEXT);
                               WEB_MESSAGE ="";

                            // on dégage en sortie envoie par exemple: fermeture page html client send("SIGTERM");
                            // pour test ecriture log reception XMLBuffer pour control conformiter voir tinyxml
                            //   if ( msg_SIGTERM == true) { fclose(f_log);   kill(getpid(), SIGKILL); }
            }

            break;
    }
    case LWS_CALLBACK_RECEIVE: {

            // log console
            lwsl_notice("received_data:>%s<\n", (char *) in);

            // traitement recuperation pour traitement XML
            line =  string( (char *) in);

            if(line != "{%" && ok== true  &&  line != "%}") XMLBuffer+= line ;

            if(line == "{%") { XMLBuffer =""; ok = true; } ; // start group xml

            if(line == "%}") {  // end group XML
                            ok = false;
                            XMLBuffer+= '\0';
                            flog = fopen("/home/soleil/WWW/htmdev/logweb.txt", "r+");
                            fputs(XMLBuffer.c_str(),flog);
                            fclose(flog);
                            read_DSPF(XMLBuffer);
                             // traitement ....

                             // return buffer to DSPF
                              WEB_MESSAGE =write_DSPF();
                            };

            if (line == "SIGTERM") {
                            lwsl_notice("SIGTERM SERVEUR\n");
                            msg_SIGTERM = true;
                            };

                   /* get notified as soon as we can write again */
                    lws_callback_on_writable(wsi);
            break;

        }

    case LWS_CALLBACK_CLOSED: {
        lwsl_notice("LWS_CALLBACK_CLOSED\n");
        Client_web --;
        printf("N° connection  %d \n" , Client_web);

        break;
    }

    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION: {
        lws_get_peer_addresses(wsi, (int)(long)in, webcon.client_name, sizeof(webcon.client_name) ,webcon.client_ip, sizeof(webcon.client_ip));
        /* controle */
        Client_web += 1;

        lwsl_notice("network connect from %s (%s)\n", webcon.client_name, webcon.client_ip);
             /* if we returned non-zero from here, we kill the connection */
        break;
        }

    };
    return 0;
}


// actuelement un seul protocol pour le test
static struct lws_protocols protocols[] = {
    {
        "echo", // service
        callback_main,
        sizeof(struct per_session_data__client), // user data struct not used
        MAX_BUFFER_SIZE,
    },{ NULL, NULL, 0, 0 } // terminator
};
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

int main(void) {
    // server url will be http://localhost:9000


    // pour test le fichier log
flog = fopen("/home/soleil/WWW/htmdev/logweb.txt", "w+");
fputs("begin",flog);
fclose(flog);
//-- new Version: context 2016
struct lws_context_creation_info info;
memset(&info, 0, sizeof info);

// renseigne la structure info

info.port = 9000;

info.iface = NULL;

info.protocols = protocols;


static const struct lws_extension exts[] = {
	{
		"permessage-deflate",
		lws_extension_callback_pm_deflate,
		"permessage-deflate; client_max_window_bits"
	},
	{
		"deflate-frame",
		lws_extension_callback_pm_deflate,
		"deflate_frame"
	},
	{ NULL, NULL, NULL /* terminator */ }
};

#ifndef LWS_NO_EXTENSIONS
    info.extensions =  exts;  // sans extension  je n'ai plus de closed intenspestive  et le susytem est  bouffé
                              // avec extension  le systeme n'est plus bouffé
                              // et le timeout dans le html est quand même obligatoire dans les 2 cas
#endif
//if (!use_ssl) {
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
//} else {
//  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
//  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
//}
info.gid = -1;
info.uid = -1;

info.options = 0;


// en laissant a zeros tout ce stabilise pour faire du temps reel et commit avec le metier
info.ka_time = 0; // 60 seconds until connection is suspicious
info.ka_probes = 0; // 10 probes after ^ time
info.ka_interval =0; // 10s interval for sending probes




context = lws_create_context(&info);


    if (context == NULL) {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }

    printf("starting server...\n");

    // infinite loop
    while (1) {
    if ( msg_SIGTERM == true  ) {
            WEB_MESSAGE = "CLOSE-SERVEUR";
            lws_callback_on_writable_all_protocol(context,&protocols[0]);

    };
    if ( msg_SIGTERM == true    ) {  fclose(flog);
                                                        printf("connection established\n  N° client : %d\n" , Client_web );
                                                        break; };




    lws_service(context,20);
// Libwebsocket_service traitera tous les événements d'attente avec leurs fonctions
// de rappel, puis attendre 20 ms.
// (Ce qui est un seul serveur web fileté et cela permet de garder notre serveur
// de génération de charge alors qu'il ya pas de demandes à traiter)
    }

lws_context_destroy(context);

    return 0;
}




bool read_DSPF( string  filexml)
{
 printf("000\n");
    XMLDocument  doc ;
  //  doc.LoadFile("/home/soleil/WWW/htmdev/logweb.txt");
    doc.Parse( filexml.c_str());
 printf("001\n");

    int errorID = doc.ErrorID();
    // doc.Print();
    if(errorID!=0){
        cerr << "erreur lors du chargement" << endl;
        cerr << "error #" << doc.ErrorID() << " : " << doc.GetErrorStr1 () << endl;
  //  return false;
    }


    XMLElement  *root  = doc.FirstChildElement( "DSPF" );

    RCDFMT = root->FirstChildElement("RCDFMT")->GetText();


           XMLElement  *ROW = root->FirstChildElement("ZONED")->FirstChildElement();

              while (ROW){
               DATE     = ROW->Attribute("DATE");
               NOM      = ROW->Attribute("NOM");
               PRENOM   = ROW->Attribute("PRENOM");
               NRUE     = ROW->Attribute("NRUE");
               RUE1     = ROW->Attribute("RUE1");
               RUE2     = ROW->Attribute("RUE2");
               ROW      = ROW->NextSiblingElement(); // iteration
            }

                           printf("\ndate:%s\n",DATE.c_str());
                           cout<<"\nnom:"<<NOM<<"\n"<<endl;


   return true;
}



void build_entete_doc( XMLDocument * doc, XMLElement  * corpx , string elem, string texte )
{



    XMLElement  *msg = doc->NewElement( elem.c_str() );
    msg->LinkEndChild( doc->NewText( texte.c_str() ));
    corpx->LinkEndChild( msg);
}


void build_field_doc( XMLElement  * atr  , string elem, string texte )
{

    atr->SetAttribute(elem.c_str(), texte.c_str());
}

string write_DSPF()
{
 printf("002");
    XMLDocument doc;
    XMLDeclaration * decl = doc.NewDeclaration();
    doc.LinkEndChild( decl );
    XMLElement  * root = doc.NewElement( "DSPF" );
    doc.LinkEndChild( root );
    build_entete_doc(&doc, root, "RCDFMT", RCDFMT.c_str() ) ;

    XMLElement  * ZONED = doc.NewElement( "ZONED" );
    root->LinkEndChild( ZONED);

    XMLElement  * atr_ZONED= doc.NewElement( "row" );

       build_field_doc(atr_ZONED , "DATE", DATE.c_str() );
       build_field_doc(atr_ZONED , "NOM", NOM.c_str() );
       build_field_doc(atr_ZONED , "PRENOM", PRENOM.c_str() );
       build_field_doc(atr_ZONED , "NRUE", NRUE.c_str() );
       build_field_doc(atr_ZONED , "RUE1", RUE1.c_str() );
       build_field_doc(atr_ZONED , "RUE2", RUE2.c_str() );
     ZONED->LinkEndChild( atr_ZONED);

       //test erreur
       if (RUE2 == "" ) { build_entete_doc( &doc, root, "FOOTER", "veuilez corriger les erreurs" ) ;
                            build_entete_doc(&doc, root, "ERRID", "RUE2" ) ;
                            build_entete_doc(&doc, root, "ERRMSG", "cette zone est obligatoire" ) ;
                        }


    // Declare a printer
    XMLPrinter  printer;
    // attach it to the document you want to convert in to a std::string
    doc.Accept(&printer);

// return your document data in to the string

return  printer.CStr ();
}
