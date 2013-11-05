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

//TODO: if config file is not available?


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

void set_log_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	extern int nfqp_logging_level, nfqp_printf_log_lvl;
	extern int glb_logging_level, glb_printf_log_lvl;

	attr = xmlGetProp(cur, "global");
	nfqp_logging_level = atoi((char*) attr);
	printf("Global: %s\n", attr);
	xmlFree(attr);

	attr = xmlGetProp(cur, "nfq");
	nfqp_logging_level = atoi((char*) attr);
	printf("nfq: %s\n", attr);
	xmlFree(attr);

	attr = xmlGetProp(cur, "globalonscreen");
	glb_printf_log_lvl = atoi((char*) attr);
	printf("globalonscreen: %s\n", attr);
	xmlFree(attr);

	attr = xmlGetProp(cur, "nfqonscreen");
	nfqp_printf_log_lvl = atoi((char*) attr);
	printf("nfqonscreen: %s\n", attr);
	xmlFree(attr);

	return;
}

void set_queue_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	extern int nfqp_queue_num;

	attr = xmlGetProp(cur, "number");
	nfqp_queue_num = atoi((char*) attr);
	printf("number: %s\n", attr);
	xmlFree(attr);

	return;
}

void set_gui_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	extern int gui_port;

	attr = xmlGetProp(cur, "port");
	gui_port = atoi((char*) attr);
	printf("port: %s\n", attr);
	xmlFree(attr);

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
			set_log_attributes(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "Queue"))) {
			set_queue_attributes(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "GUI_socket"))) {
			set_gui_attributes(doc, cur);
		}

		cur = cur->next;
	}

	xmlFreeDoc(doc);
	return;
}



