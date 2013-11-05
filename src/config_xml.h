/*
 ============================================================================
 Name        : config_xml.h
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Reads the XML formatted configuration file
 ============================================================================
 */
#ifndef CONFIG_XML_H_
#define CONFIG_XML_H_

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

int cfg_init() ;

void parse_logging(xmlDocPtr doc, xmlNodePtr cur) ;

void set_log_parameters(xmlDocPtr doc, xmlNodePtr cur) ;

void set_queue_attributes(xmlDocPtr doc, xmlNodePtr cur) ;

void set_gui_attributes(xmlDocPtr doc, xmlNodePtr cur);

void get_log(xmlDocPtr doc, xmlNodePtr cur) ;

void parse_doc(char *docname);



#endif /* CONFIG_XML_H_ */
