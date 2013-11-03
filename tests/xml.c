#include <stdio.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

static void 
print_element_names(xmlNode * a_node)
 {
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node =
         cur_node->next) {
       if (cur_node->type == XML_ELEMENT_NODE) {
          printf("node type: Element, name: %s\n",
               cur_node->name);
       }
       print_element_names(cur_node->children);
    }
 }



void 
parse_xml_elements(xmlNode * a_node){

	xmlNode *cur = NULL;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo"))){
			parseLogging (doc, cur);
		}
		 
	cur = cur->next;
	}
	

}

void
getReference (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *uri;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"reference"))) {
		    uri = xmlGetProp(cur, "uri");
		    printf("uri: %s\n", uri);
		    xmlFree(uri);
	    }
	    cur = cur->next;
	}
	return;
}

 void parse_logging (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *key;
	 cur = cur->xmlChildrenNode;
	 while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"keyword"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("keyword: %s\n", key);
		    xmlFree(key);
 	    }
	cur = cur->next;
	}
    return;
}




 int main(int argc, char **argv)
 {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    if (argc != 2)  return(1);

    LIBXML_TEST_VERSION    // Macro to check API for match with
                             // the DLL we are using

   /*parse the file and get the DOM */
   if ((doc = xmlReadFile(argv[1], NULL, 0)) == NULL){
       printf("error: could not parse file %s\n", argv[1]);
       exit(-1);
      }

   /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);
	
	// Checking if its the <Config> the root Node.

	if (xmlStrcmp(root_element->name, (const xmlChar *) "Config")) {
			fprintf(stderr,"document of the wrong type, root node != Config");
			xmlFreeDoc(doc);
			return;
		}

    print_element_names(root_element);

    xmlFreeDoc(doc);       // free document 
   xmlCleanupParser();    // Free globals
    return 0;
 }
