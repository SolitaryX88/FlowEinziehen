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

void print_log_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	attr = xmlGetProp(cur, "global");
	printf("Global: %s\n", attr);
	xmlFree(attr);

	attr = xmlGetProp(cur, "nfq");
	printf("nfq: %s\n", attr);
	xmlFree(attr);

	attr = xmlGetProp(cur, "globalonscreen");
	printf("globalonscreen: %s\n", attr);
	xmlFree(attr);

	attr = xmlGetProp(cur, "nfqonscreen");
	printf("nfqonscreen: %s\n", attr);
	xmlFree(attr);

	return;
}

void print_queue_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	attr = xmlGetProp(cur, "number");
	printf("number: %s\n", attr);
	xmlFree(attr);

	return;
}

void print_gui_attributes(xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *attr;

	attr = xmlGetProp(cur, "port");
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
	xmlChar *uri;

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
			print_log_attributes(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "Queue"))) {
			//parse_queue (doc, cur);
			print_queue_attributes(doc, cur);
		} else if ((!xmlStrcmp(cur->name, (const xmlChar *) "GUI_socket"))) {
			//parse_gui_socket (doc, cur);
			print_gui_attributes(doc, cur);
		}

		cur = cur->next;
	}

	xmlFreeDoc(doc);
	return;
}



