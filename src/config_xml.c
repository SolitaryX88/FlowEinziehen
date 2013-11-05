/*
 ============================================================================
 Name        : config_xml.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Reads the XML formatted configuration file
 ============================================================================
 */

#include "global.h"
#include "config_xml.h"

#define getIntProp(xmlCharVar,xmlAttribute,xmlNodePtrVar,setVariable)\
	xmlCharVar=xmlGetProp(xmlNodePtrVar,xmlAttribute);\
	setVariable=atoi((char*)xmlCharVar);\
	xmlFree(xmlCharVar);

//TODO: if config file is not available?


extern int gui_port;

extern int nfqp_logging_level, nfqp_printf_log_lvl;
extern int glb_logging_level, glb_printf_log_lvl;

extern int nfqp_queue_num;


int cfg_init() {

	return (SUCCESS);
}

void //Not currently in use. Maybe in future
parse_logging(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL ) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "FileName"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("FileName: %s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
	}
	return;
}



void set_log_parameters(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	cur = cur->xmlChildrenNode;

	while (cur != NULL ) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "global"))) {
			getIntProp(attr, "log_lvl", cur, glb_logging_level)
			getIntProp(attr, "printf_lvl", cur, glb_printf_log_lvl)

		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "nfq"))) {
			getIntProp(attr, "log_lvl", cur, nfqp_logging_level)
			getIntProp(attr, "printf_lvl", cur, nfqp_printf_log_lvl)

		}
		cur = cur->next;
	}

	return;
}

void set_queue_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	getIntProp(attr, "number", cur, nfqp_queue_num)

	return;
}

void set_gui_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	getIntProp(attr, "port", cur, gui_port)

	return;
}

void get_log(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *uri;
	cur = cur->xmlChildrenNode;
	while (cur != NULL ) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "reference"))) {
			uri = xmlGetProp(cur, "uri");
			printf("uri: %s\n", uri);
			xmlFree(uri);
		}
		cur = cur->next;
	}
	return;
}

void parse_doc(char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL ) {
		fprintf(stderr, "Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL ) {
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "Config")) {
		fprintf(stderr, "document of the wrong type, root node != Config");
		xmlFreeDoc(doc);
		return;
	}

	// After checking that our root element root is "Config" now we parse the re children elements
	cur = cur->xmlChildrenNode;

	while (cur != NULL ) {

		if ((!xmlStrcmp(cur->name, (const xmlChar *) "Logging"))) {
			set_log_parameters(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "Queue"))) {
			set_queue_attributes(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "GUI_socket"))) {
			set_gui_attributes(doc, cur);
		}

		cur = cur->next;
	}

	printf("Cfg: Global(log, print): %d %d \t NFQ(log, print): %d %d\n", glb_logging_level, glb_printf_log_lvl, nfqp_logging_level, nfqp_printf_log_lvl);


	xmlFreeDoc(doc);
	return;
}



