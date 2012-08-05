<?php

/**
 * @file
 * Winbank 3DSecure menu items.
 *
 */

function uc_winbank_3dsecure_response(){

	// Loading CenitelClient.inc
	module_load_include('inc', 'uc_winbank_3dsecure', 'centinel/CentinelClient');
	
	// Get the values of required variables	
	$uc_centinel_processor_id = variable_get('uc_centinel_processor_id',''); 
	$uc_centinel_merchant_id = variable_get('uc_centinel_merchant_id',''); 
	$uc_centinel_transaction_password = variable_get('uc_centitel_transaction_pwd',''); 
	//$uc_centinel_transaction_password = md5($uc_centinel_transaction_password);
	$uc_centinel_maps_url = variable_get('uc_centinel_maps_url',''); 
	$uc_centinel_demo_url = variable_get('uc_centinel_demo_url',''); 
	
	
	$uc_winbank_request_type = variable_get('uc_winbank_request_type',''); 
	$uc_winbank_acquirer_id = variable_get('uc_winbank_acquirer_id',''); 
	$uc_winbank_merchant_id = variable_get('uc_winbank_merchant_id','');
	$uc_winbank_pos_id = variable_get('uc_winbank_pos_id',''); 
	$uc_winbank_channel_type = variable_get('uc_winbank_channel_type','');
	$uc_winbank_user = variable_get('uc_winbank_user','');
	$uc_winbank_password = variable_get('uc_winbank_password','');
	$uc_winbank_password = md5($uc_winbank_password);
	$uc_winbank_currency_code = variable_get('uc_winbank_currency_code','');
	$uc_winbank_transaction_url = variable_get('uc_winbank_transaction_url','');
<<<<<<< HEAD
	
	//$demo_mode = 
=======
>>>>>>> origin/master
	drupal_set_title("Πληροφορίες συναλλαγής");
	
	/*
	*	Get parameters from Centinel
	*/
	$MD_Paygate = $_POST['MD'];
	$PaRes = $_POST['PaRes'];
	$transactionType = "C";
	$order_number = $_SESSION['3d_winbank']['order_number'];
	
	
	/* Creating a New Centinel Client
	* cmpi_authenticate
	*/
	$centinelClient = new CentinelClient;
	
	$centinelClient->add('MsgType', 'cmpi_authenticate');
	$centinelClient->add('Version', '1.7');
	$centinelClient->add('MerchantId', $uc_centinel_merchant_id);
	$centinelClient->add('ProcessorId', $uc_centinel_processor_id);
	$centinelClient->add('TransactionPwd', $uc_centinel_transaction_password);
	$centinelClient->add('TransactionType', $transactionType);
	$centinelClient->add('TransactionId', $_SESSION['3d_winbank']['Centinel_TransactionId']);
	$centinelClient->add('PAResPayload', $PaRes);
	
	$centinel_time_out_connect = 50;
	$centinel_timeout_read = 100;
	$centinelClient->sendHttp($uc_centinel_maps_url, $centinel_time_out_connect, $centinel_timeout_read);
	
	
	// Using the centinelResponse object, we need to retrieve the results as follows
	$Centinel_ErrorDesc = $centinelClient->getValue("ErrorDesc");
	$Centinel_ErrorNo = $centinelClient->getValue("ErrorNo");
	$Centinel_PAResStatus = $centinelClient->getValue("PAResStatus");
	$Centinel_SignatureVerification = $centinelClient->getValue("SignatureVerification");
	$Centinel_CAVV = $centinelClient->getValue("Cavv");
	$Centinel_ECI = $centinelClient->getValue("EciFlag");
	$Centinel_XID = $centinelClient->getValue("Xid");
<<<<<<< HEAD
	
	
	//echo "$Centinel_ErrorDesc -- $Centinel_ErrorNo";
=======
>>>>>>> origin/master
												
	/**
	*	Παίρνουμε τις μεταβλητές της συναλλαγής απο τα SESSIONS
	*/
	
	$order_number = $_SESSION['3d_winbank']['order_number'];
	$amount = $_SESSION['3d_winbank']['amount'];
	$cc_type = $_SESSION['3d_winbank']['card_type'];
	$cardHolderName = $_SESSION['3d_winbank']['cardHolderName'];
	$orderDesc = $cardHolderName;
	//$cardHolderName = "Test01";
	$expr_mm = $_SESSION['3d_winbank']['expr_mm'];
	$expr_yyyy = $_SESSION['3d_winbank']['expr_yyyy'];
	$cc_ccv = $_SESSION['3d_winbank']['ccv'];
	$enrolled = $_SESSION['3d_winbank']['enrolled'];
	$card_number = $_SESSION['3d_winbank']['card_number'];
	
	$key = uc_credit_encryption_key();
   	$crypt = new uc_encryption_class;  		
    $card_number = unserialize($crypt->decrypt($key, $card_number));
	//echo $cc_number;
	
	$order_id = $_SESSION['3d_winbank']['order_id'];
<<<<<<< HEAD
	

=======
>>>>>>> origin/master
	drupal_get_messages('error');
	
	if($Centinel_ErrorNo == '0' AND $Centinel_SignatureVerification == "Y" && $Centinel_PAResStatus != "N" && $Centinel_PAResStatus != "U"){
	        	
	  															        
		/*
		*	START of Authorization with WINBANK-
		*/
		
		
		// Setting the proper Values for the Winbank Web Service		
		if($Centinel_PAResStatus == "Y"){
		   $Centinel_PAResStatus = "Succeded";
		}else if($Centinel_PAResStatus == "N"){
		   $Centinel_PAResStatus = "Failed";
		}else if($Centinel_PAResStatus == "A"){
		   $Centinel_PAResStatus = "Attempted";
		}else if($Centinel_PAResStatus == "U"){
		   $Centinel_PAResStatus = "Unknown";
		}
		
		
		
		if($Centinel_SignatureVerification == "Y"){
		    $Centinel_SignatureVerification = "Yes";
		}elseif($Centinel_SignatureVerification == "N"){
		    $Centinel_SignatureVerification = "No";
		}elseif($Centinel_SignatureVerification == "U"){
		 	$Centinel_SignatureVerification = "Undefined";
		}
		$preauth_data = "<?xml version='1.0' encoding='utf-8'?>
		<soap:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap='http://schemas.xmlsoap.org/soap/envelope/'>
		<soap:Body>
		<ProcessTransaction xmlns='http://piraeusbank.gr/paycenter'>
		<TransactionRequest xmlns='http://piraeusbank.gr/paycenter/1.0'>
		<Header>
		<RequestType>$uc_winbank_request_type</RequestType>
		<MerchantInfo>
		<AcquirerID>$uc_winbank_acquirer_id</AcquirerID>
		<MerchantID>$uc_winbank_merchant_id</MerchantID>
		<PosID>$uc_winbank_pos_id</PosID>
		<ChannelType>3DSecure</ChannelType>
		<User>$uc_winbank_user</User>
		<Password>$uc_winbank_password</Password>
		</MerchantInfo>
		
		</Header>
		<Body>
		<TransactionInfo>
		<MerchantReference>$order_number</MerchantReference>
		<TransactionReferenceID xsi:nil='true' />
		<EntryType xsi:nil='true' />
		<CurrencyCode>$uc_winbank_currency_code</CurrencyCode>
		<Amount>$amount</Amount>				
		<Installments xsi:nil='true' />					
		<ExpirePreauth xsi:nil='true' />
		<TipAmount xsi:nil='true' />
		<Bnpl xsi:nil='true' />
		<SessionKey xsi:nil='true' />
		<CardInfo><CardType>$cc_type</CardType>
		<CardNumber>$card_number</CardNumber>
		<CardHolderName>$cardHolderName</CardHolderName>
		<ExpirationMonth>$expr_mm</ExpirationMonth>
		<ExpirationYear>$expr_yyyy</ExpirationYear>
		<Cvv2>$cc_ccv</Cvv2>
		<Aid xsi:nil='true' />
		<PinBlock xsi:nil='true' />
		<Track1 xsi:nil='true' />
		<Track2 xsi:nil='true' />
		</CardInfo><AuthInfo>
		<Cavv>$Centinel_CAVV</Cavv>
		<Eci>$Centinel_ECI</Eci>
		<Xid>$Centinel_XID</Xid>
		<Enrolled>$enrolled</Enrolled>
		<PAResStatus>$Centinel_PAResStatus</PAResStatus>
		<SignatureVerification>$Centinel_SignatureVerification</SignatureVerification>
		</AuthInfo>
		</TransactionInfo>
		</Body>
		</TransactionRequest></ProcessTransaction></soap:Body></soap:Envelope>";

		$ch = curl_init($uc_winbank_transaction_url);

		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSLVERSION, 3);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 90); 
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: text/xml'));
		curl_setopt($ch, CURLOPT_POSTFIELDS, "$preauth_data");
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		$output = curl_exec($ch);
		curl_close($ch);
		
		
		$xmlString = preg_replace("/(<\/?)(\w+):([^>]*>)/", "$1$2$3", $output);
		 
		// Use SimpleXML to parse response into object
		$xml = new SimpleXMLElement($xmlString);
		 
		// SimpleXML object for tags in the <soap:Body> node
		$result = $xml->soapBody;
		
		$statusFlag = $result->ProcessTransactionResponse->TransactionResponse->Body->TransactionInfo->StatusFlag;
		$resultDescription = $result->ProcessTransactionResponse->TransactionResponse->Header->ResultDescription;
		$resultCode = $result->ProcessTransactionResponse->TransactionResponse->Header->ResultCode;
		$responceCode = $result->ProcessTransactionResponse->TransactionResponse->Body->TransactionInfo->ResponseCode;
		$supportReferenceID = $result->ProcessTransactionResponse->TransactionResponse->Header->SupportReferenceID;
		
<<<<<<< HEAD
		/*
		echo "StatusFlag-> $statusFlag | desc -> $resultDescription<br/>$resultCode<br/>$responceCode";
		
		$omg = $bankPreAuthResponse->TransactionResponse->RequestType;
		echo "$omg<br/>";
		echo "<pre>";
		var_dump($output);
		echo "</pre>";
		*/
		
		
=======
>>>>>>> origin/master
		// Παιρνάμε τις τιμές στο table uc_winbank_3dsecure_test_cases
		db_query("INSERT INTO uc_winbank_3dsecure_test_cases (cc_type, orderNumber, supportReferenceID,cardHolderName) VALUES 
		('$cc_type','$order_number','$supportReferenceID','$orderDesc')");
		
		unset($_SESSION['3d_winbank']);		
		
		if($resultCode == '0' AND $responceCode == '00' AND $statusFlag == "Success"){
		
		//Complete Reservation insert required values		
<<<<<<< HEAD
		$comment = t('Η συναλλαγή σας ολοκληρώθηκε με επιτυχία.');
=======
		$comment = "Η αγορά ολοκληρώθηκε με επιτυχία.";
>>>>>>> origin/master
	    uc_payment_enter($order_id, 'uc_winbank_3dsecure', $amount, 0, NULL, $comment);
		uc_order_comment_save($order_id, 0, t('Order created through website.'), 'admin');
		$_SESSION['do_complete'] = TRUE;
		drupal_goto('cart/checkout/complete');
		
		}else if($resultCode == '0' AND $responceCode == '12' AND $statusFlag == "Failure"){
			// Declined
<<<<<<< HEAD
			$message = t('Η συναλλαγή σας απορρίφθηκε απο την τράπεζα');		   
=======
			$message = 'Η συναλλαγή σας απορρίφθηκε απο την τράπεζα';		   
>>>>>>> origin/master
		    drupal_set_message($message,'error');
		    $_SESSION['do_review'] = TRUE;
		    drupal_goto('cart/checkout/review');	
		    
		}else if($resultCode == '0' AND $responceCode == '11') {	
			// Double Charge
<<<<<<< HEAD
			$message = t('Απόπειρα διπλοχρέωσης συναλλαγής. Η συναλλαγή ακυρώθηκε.');		   
=======
			$message = 'Απόπειρα διπλοχρέωσης συναλλαγής. Η συναλλαγή ακυρώθηκε.';		   
>>>>>>> origin/master
		    drupal_set_message($message,'error');
		    $_SESSION['do_review'] = TRUE;
		    drupal_goto('cart/checkout/review');
		    
		}else if($resultCode == '501'){
			// Technical Problem
<<<<<<< HEAD
			$message = t('Αδυναμία εκτέλεσης συναλλαγής λόγω (τεχνικού) προβλήματος επικοινωνίας με το σύστημα επεξεργασίας συναλλαγών.');		   
=======
			$message = 'Αδυναμία εκτέλεσης συναλλαγής λόγω (τεχνικού) προβλήματος επικοινωνίας με το σύστημα επεξεργασίας συναλλαγών.';		   
>>>>>>> origin/master
		    drupal_set_message($message,'error');
		    $_SESSION['do_review'] = TRUE;
		    drupal_goto('cart/checkout/review');
		    
		}else if($resultCode == '981'){
			// Wrong Card Type
<<<<<<< HEAD
			$message = t('Αδυναμία εκτέλεσης της συναλλαγής λόγω λανθασμένων στοιχείων κάρτας ή κάρτας που δεν υποστηρίζεται απο το σύστημα');		   
=======
			$message = 'Αδυναμία εκτέλεσης της συναλλαγής λόγω λανθασμένων στοιχείων κάρτας ή κάρτας που δεν υποστηρίζεται απο το σύστημα';		   
>>>>>>> origin/master
		    drupal_set_message($message,'error');
		    $_SESSION['do_review'] = TRUE;
		    drupal_goto('cart/checkout/review');
			 
		 
		}else{
				 
<<<<<<< HEAD
			$message = t('Αδυναμία εκτέλεσης της συναλλαγής.');		   
=======
			$message = 'Αδυναμία εκτέλεσης της συναλλαγής.';		   
>>>>>>> origin/master
		    drupal_set_message($message,'error');
		    $_SESSION['do_review'] = TRUE;
		    drupal_goto('cart/checkout/review');
		}
	
	}else if($Centinel_ErrorNo == '1050'){
		
		db_query("INSERT INTO uc_winbank_3dsecure_test_cases (cc_type, orderNumber, supportReferenceID,cardHolderName) VALUES 
		('$cc_type','$order_number','$supportReferenceID','$orderDesc')");		
			
		unset($_SESSION['3d_winbank']);
		
		$message = t('Αδυναμία εκτέλεσης της συναλλαγής λόγω προβλήματος στο σύστημα');		   
		drupal_set_message($message);
		$_SESSION['do_review'] = TRUE;
		drupal_goto('cart/checkout/review');
		
	}else{
		
		db_query("INSERT INTO uc_winbank_3dsecure_test_cases (cc_type, orderNumber, supportReferenceID,cardHolderName) VALUES 
		('$cc_type','$order_number','$supportReferenceID','$orderDesc')");
		
		unset($_SESSION['3d_winbank']);
		
<<<<<<< HEAD
		$message = t('Αδυναμία εκτέλεσης της συναλλαγής.');		   
=======
		$message = 'Αδυναμία εκτέλεσης της συναλλαγής.-';		   
>>>>>>> origin/master
		drupal_set_message($message,'error');
		$_SESSION['do_review'] = TRUE;
		drupal_goto('cart/checkout/review');	
	}
<<<<<<< HEAD
/*
echo "<br/> <br/><br/><br/><br/><br/>$resultCode -- $responceCode -- $statusFlag || RESULT DESCRIPTION -> $resultDescription<br/><br/><br/><br/><br/>";
$output = "<br/> <br/><br/><br/><br/><br/>$resultCode -- $responceCode -- $statusFlag || RESULT DESCRIPTION -> $resultDescription<br/><br/><br/><br/><br/>";	
return $output;	
*/
=======
//echo "<br/> <br/><br/><br/><br/><br/>$resultCode -- $responceCode -- $statusFlag || RESULT DESCRIPTION -> $resultDescription<br/><br/><br/><br/><br/>";
//$output = "<br/> <br/><br/><br/><br/><br/>$resultCode -- $responceCode -- $statusFlag || RESULT DESCRIPTION -> $resultDescription<br/><br/><br/><br/><br/>";	
//return $output;	

>>>>>>> origin/master
}



