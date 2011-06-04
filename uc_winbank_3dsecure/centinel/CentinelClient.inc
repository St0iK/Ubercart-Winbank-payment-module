<?php 

/////////////////////////////////////////////////////////////////////////////////////////////
//  CardinalCommerce (http://www.cardinalcommerce.com)
//  CentinelClient.php
//  Version 1.2 02/17/2005
//
//	Usage
//		The CentinelClient class is defined to assist integration efforts with the Centinel
//		XML message integration. The class implements helper methods to construct, send, and
//		receive XML messages with respect to the Centinel XML Message APIs.
//
////////////////////////////////////////////////////////////////////////////////////////////

class XMLParser{

		var $xml_parser;
		var $deseralizedResponse;
		var $elementName;
		var $elementValue;
       
	    /////////////////////////////////////////////////////////////////////////////////////////////
		// Function XMLParser()
		//
		// Initialize the XML parser. 
		/////////////////////////////////////////////////////////////////////////////////////////////

		function XMLParser() {
		  $this->xml_parser = xml_parser_create();
		}
   
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function startElement(parser, name, attribute)
		//
		// Start Tag Element Handler
		/////////////////////////////////////////////////////////////////////////////////////////////
		
		function startElement($parser, $name, $attrs='') { 
			$this->elementName= $name;
			
		} 
   
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function elementData(parser, data)
		//
		// Element Data Handler 
		/////////////////////////////////////////////////////////////////////////////////////////////

		function elementData($parser, $data) { 
			$this->elementValue .= $data;
			
		} 
		
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function endElement(name, value)
		//
		// End Tag Element Handler
		/////////////////////////////////////////////////////////////////////////////////////////////

		function endElement($parser, $name) { 
								 
			 $this->deserializedResponse[$this->elementName]= $this->elementValue;
			 $this->elementName = "";
			 $this->elementValue = "";
		} 
   
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function deserialize(xmlString)
		//
		// Deserilize the XML reponse message and add each element to the deseralizedResponse collection.
		// Once complete, then each element reference will be available using the getValue function.
		/////////////////////////////////////////////////////////////////////////////////////////////

		function deserializeXml($responseString) {
   
			  xml_set_object($this->xml_parser, $this);
			  xml_parser_set_option($this->xml_parser,XML_OPTION_CASE_FOLDING,FALSE);
			  xml_set_element_handler($this->xml_parser, "startElement", "endElement");
			  xml_set_character_data_handler($this->xml_parser, "elementData");

			  if (!xml_parse($this->xml_parser, $responseString)) {
			  
					$this->deserializedResponse["ErrorNo"]= CENTINEL_ERROR_CODE_8020;
					$this->deserializedResponse["ErrorDesc"]= CENTINEL_ERROR_CODE_8020_DESC;
			  }
	   
			  xml_parser_free($this->xml_parser);
		}
 }
	 define("CENTINEL_ERROR_CODE_8000", "8000");
    define("CENTINEL_ERROR_CODE_8000_DESC", "Protocol Not Recogonized, must be http:// or https://");
	 define("CENTINEL_ERROR_CODE_8010", "8010");
    define("CENTINEL_ERROR_CODE_8010_DESC", "Unable to Communicate with MAPS Server");
	 define("CENTINEL_ERROR_CODE_8020", "8020");
    define("CENTINEL_ERROR_CODE_8020_DESC", "Error Parsing XML Response");
	 define("CENTINEL_ERROR_CODE_8030", "8030");
    define("CENTINEL_ERROR_CODE_8030_DESC", "Communication Timeout Encountered");
	

    class CentinelClient {
   
		var $request ;
		var $response ;  
		var $parser;
    
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function Add(name, value)
		//
		// Add name/value pairs to the Centinel request collection. 
		/////////////////////////////////////////////////////////////////////////////////////////////

		
		function add($name, $value) {
			 $this->request[$name] = $this->escapeXML($value);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function getValue(name)
		//
		// Retrieve a specific value for the give name within the Centinel response collection. 
		/////////////////////////////////////////////////////////////////////////////////////////////


		function getValue($name) {
			return $this->response[$name];
		}
	   

		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function getRequestXml(name)
		//
		// Serialize all elements of the request collection into a XML message, and format the required
		// form payload according to the Centinel XML Message APIs. The form payload is returned from  
		// the function.
		/////////////////////////////////////////////////////////////////////////////////////////////

		
		function getRequestXml(){
			$queryString = "<CardinalMPI>";
			foreach ($this->request as $name => $value) {
				$queryString = $queryString."<".($name).">".($value)."</".($name).">" ;
			}
			$queryString = $queryString."</CardinalMPI>";
			return "cmpi_msg=".urlencode($queryString);
		}
	   
	    /////////////////////////////////////////////////////////////////////////////////////////////
		// Function sendHttp(url, "", $timeout)
		//
		// HTTP POST the form payload to the url using cURL.
		// form payload according to the Centinel XML Message APIs. The form payload is returned from  
		// the function.
		/////////////////////////////////////////////////////////////////////////////////////////////

		function sendHttp($url, $connectTimeout="", $timeout) {
		   
		    // verify that the URL uses a supported protocol.

			if( (strpos($url, "http://")=== 0) || (strpos($url, "https://")=== 0) ) {
					 
				//Construct the payload to POST to the url.

				$data = $this->getRequestXml();
			
				// create a new cURL resource

				$ch = curl_init($url);

				// set URL and other appropriate options

				curl_setopt($ch, CURLOPT_POST,1);
				curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
				curl_setopt($ch, CURLOPT_SSL_VERIFYHOST,  2);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
				curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);  
				curl_setopt($ch, CURLOPT_TIMEOUT, $timeout); 
						  
				// Execute the request.

				$result = curl_exec($ch);
				$succeeded  = curl_errno($ch) == 0 ? true : false;
				
				// close cURL resource, and free up system resources

				curl_close($ch); 
				
				// If Communication was not successful set error result, otherwise 
		
				if(!$succeeded) {

					$result = $this->setErrorResponse(CENTINEL_ERROR_CODE_8030, CENTINEL_ERROR_CODE_8030_DESC);

				} 

				// Assert that we received an expected Centinel Message in reponse.

				if (strpos($result, "<CardinalMPI>") === false) {
					$result = $this->setErrorResponse(CENTINEL_ERROR_CODE_8010, CENTINEL_ERROR_CODE_8010_DESC);
				}
							
					
			} else {
				$result = $this->setErrorResponse(CENTINEL_ERROR_CODE_8000, CENTINEL_ERROR_CODE_8000_DESC);
			}
			$parser = new XMLParser;
			$parser->deserializeXml($result);
			$this->response = $parser->deserializedResponse;
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function setErrorResponse(errorNo, errorDesc)
		//
		// Initialize an Error response to ensure that parsing will be handled properly.
		/////////////////////////////////////////////////////////////////////////////////////////////

		function setErrorResponse($errorNo, $errorDesc) {
		
		  $resultText  = "<CardinalMPI>";
		  $resultText = $resultText."<ErrorNo>".($errorNo)."</ErrorNo>" ;
		  $resultText = $resultText."<ErrorDesc>".($errorDesc)."</ErrorDesc>" ;
		  $resultText  = $resultText."</CardinalMPI>";
		  
		  return $resultText;
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Function escapeXML(value)
		//
		// Escaped string converting all '&' to '&amp;' and all '<' to '&lt'. Return the escaped value.
		/////////////////////////////////////////////////////////////////////////////////////////////

		function escapeXML($elementValue){
		
			$escapedValue = str_replace("&", "&amp;", $elementValue);
			$escapedValue = str_replace("<", "&lt;", $escapedValue);
			
			return $escapedValue;
		
		}
	
	}
?>