#include "R3BCaloPreamplifierPar.h"

#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"            // for FairDbStatement

#include "FairParamList.h"              // for FairParamList

#include "Riosfwd.h"                    // for ostream
#include "TString.h"                    // for TString

#include <stdlib.h>                     // for exit
#include <memory>                       // for auto_ptr, etc
#include <vector>                       // for vector, vector<>::iterator


using namespace std;

ClassImp(R3BCaloPreamplifierPar);

#include "FairDbReader.tpl"
template class  FairDbReader<R3BCaloPreamplifierPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<R3BCaloPreamplifierPar>;




R3BCaloPreamplifierPar::R3BCaloPreamplifierPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbObjTableMap(name,title,context, own),
    fId(""),
    fSerialNumber(""),
    fRange(""),   
    fOrigin(""),
    fLocation(""),
    fObservation(""),
    fDbEntry(0),  // Set the default Db Entry to the first slot
    fParam_Writer(NULL), //  Writer Meta-Class for SQL IO
    fParam_Reader(NULL), //  Reader Meta-Class for SQL IO
    fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  
}


R3BCaloPreamplifierPar::~R3BCaloPreamplifierPar()
{
  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=NULL;
  }

  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=NULL;
  }

}


void R3BCaloPreamplifierPar::putParams(FairParamList* list)
{
  std::cout<<"-I- R3BCaloPreamplifierPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("PreamplifierPar_Id", (Text_t*)fId.c_str());
  list->add("PreamplifierPar_SerialNumber", (Text_t*)fSerialNumber.c_str());
  list->add("PreamplifierPar_Range", (Text_t*)fRange.c_str());
  list->add("PremaplifierPar_Origin", (Text_t*)fOrigin.c_str());
  list->add("PreamplifierPar_Location", (Text_t*)fLocation.c_str());
  list->add("PreamplifierPar_Observation", (Text_t*)fObservation.c_str());
}

Bool_t R3BCaloPreamplifierPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  Text_t idaux[80];
  if (!list->fill("PreamplifierPar_Id", idaux, 80)) { return kFALSE; }
  fId = idaux;
  Text_t serialnumberaux[80];
  if (!list->fill("PreamplifierPar_SerialNumber", serialnumberaux, 80)) { return kFALSE; }
  fSerialNumber = serialnumberaux;
  Text_t rangeaux[80];
  if (!list->fill("PreamplifierPar_Range", rangeaux, 80)) { return kFALSE; }
  fRange = rangeaux;
  Text_t originaux[80];
  if (!list->fill("PreamplifierPar_Origin", originaux, 80)) { return kFALSE; }
  fOrigin = originaux;
  Text_t locationaux[80];
  if (!list->fill("PreamplifierPar_Location", locationaux, 80)) { return kFALSE; }
  fLocation = locationaux;
  Text_t observationaux[80];
  if (!list->fill("PreamplifierPar_Observation", observationaux, 80)) { return kFALSE; }
  fObservation = observationaux;

  return kTRUE;
}

void R3BCaloPreamplifierPar::clear()
{
  fId=fSerialNumber=fRange=fOrigin=fLocation=fObservation="";

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string R3BCaloPreamplifierPar::GetTableDefinition(const char* Name)
{

  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "R3BCALOPREAMPLIFIERPAR"; }
  sql += "( SEQNO            		INT NOT NULL,";
  sql += "  ROW_ID           		INT NOT NULL,";
  sql += "  ID               		TEXT NOT NULL,";
  sql += "  SERIALNUMBER     		TEXT NOT NULL,";
  sql += "  RANGE      			TEXT NOT NULL,";
  sql += "  ORIGIN			TEXT,";
  sql += "  LOCATION			TEXT,";
  sql += "  OBSERVATION		     	TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}



void R3BCaloPreamplifierPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  clear();

  res_in >> fId  >> fSerialNumber >> fRange >> fOrigin >> fLocation >> fObservation;

}

void R3BCaloPreamplifierPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{

  res_out << fId  << fSerialNumber  << fRange << fOrigin << fLocation << fObservation;

}


void R3BCaloPreamplifierPar::fill(UInt_t rid)
{
  // Get Reader Meta Class
  fParam_Reader=GetParamReader();

  // Define a Context
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kCal,DataType::kData,ts);

  // Activate reading for this Context
  fParam_Reader->Activate(context, GetVersion());

  // By default use the latest row entry
  // (Other rows would correspond to outdated versions)
  Int_t numRows = fParam_Reader->GetNumRows();
  if ( numRows > 1 ) { numRows = 1; }

  for (int i = 0; i < numRows; ++i) {
    R3BCaloPreamplifierPar* cgd = (R3BCaloPreamplifierPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fId =  cgd->GetId();
    fSerialNumber =  cgd->GetRange();
    fRange =  cgd->GetRange();
    fOrigin =  cgd->GetOrigin();
    fLocation =  cgd->GetLocation();
    fObservation =  cgd->GetObservation();
  }

}


void R3BCaloPreamplifierPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  R3BCaloPreamplifierPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("R3BCALOPREAMPLIFIERPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("R3BCALOPREAMPLIFIERPAR").Data());
    sql_cmds.push_back(R3BCaloPreamplifierPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- R3BCaloPreamplifierPar::Store() ******* Error Executing SQL commands ***********  " << endl;
    }

  }

  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(GetDbEntry())->SetTableExists();

  // Writer Meta-Class Instance
  fParam_Writer = GetParamWriter();


  // Activate Writer Meta-Class with the proper
  // Validity Time Interval (run_id encapsulated)

  // Writer Activate() arguments list
  //                      <Arguments>                   <Type>                  <Comments>
  //
  //                      Validity Interval,            ValInterval
  //                      Composition      ,            Int_t                   set via cont. factory
  //                      Version          ,            Int_t                   set via cont. factory
  //                      DbEntry          ,            Int_t                   set via cont. factory
  //                      LogTitle         ,            std::string             set via cont. factory

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"Califa Preamplifier Node Pars");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- R3BCaloPreamplifierPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void R3BCaloPreamplifierPar::Print()
{

  cout<<  " Preamplifier Id: " << fId << endl;
  cout << " Preamplifier Serial Number: "<< fSerialNumber <<  endl;
  cout << " Preamplifier Range: " << fRange << endl;
  cout << " Preamplifier Origin : " << fOrigin << endl;
  cout << " Preamplifier Location : " << fLocation << endl;
  cout << " Preamplifier Observation : " << fObservation << endl;
}


Bool_t R3BCaloPreamplifierPar::Compare(const R3BCaloPreamplifierPar& that ) const {
  //  
  Bool_t test_h =      (fId          == that.fId)
 	               &&  (fSerialNumber   == that.fSerialNumber)
 	               &&  (fRange   == that.fRange)
 	               &&  (fOrigin   == that.fOrigin)
		       &&  (fLocation   == that.fLocation)
 	               &&  (fObservation   == that.fObservation);
  
 return (test_h); 
}


FairDbWriter<R3BCaloPreamplifierPar>* R3BCaloPreamplifierPar::ActivateWriter(Int_t rid)
{
  // delete if already existing
  if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }

  else {
	// Create according to IoV
	Bool_t fail= kFALSE;
	
	// Create a unique statement on choosen DB entry
	auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
	if ( ! stmtDbn.get() ) {
	  cout << "-E-  R3BCaloPreamplifierPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
		   << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
	  exit(1);
	}
	
	// Check if for this DB entry the table already exists.
	// If not call the corresponding Table Definition Function
	std::vector<std::string> sql_cmds;
	TString atr(GetName());
	atr.ToUpper();
	
	if (! fMultConn->GetConnection(GetDbEntry())->TableExists("R3BCALOPREAMPLIFIERPAR") ) {
	  sql_cmds.push_back(FairDb::GetValDefinition("R3BCALOPREAMPLIFIERPAR").Data());
	  sql_cmds.push_back(R3BCaloPreamplifierPar::GetTableDefinition());
	}
	
	// Packed SQL commands executed internally via SQL processor
	std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
	while( itr != itrEnd ) {
	  std::string& sql_cmd(*itr++);
	  stmtDbn->Commit(sql_cmd.c_str());
	  if ( stmtDbn->PrintExceptions() ) {
		fail = true;
		cout << "-E- R3BCaloPreamplifierPar::Store() ******* Error Executing SQL commands ***********  " << endl;
	  }
	  
	}
	
	// Refresh list of tables in connected database
	// for the choosen DB entry
	fMultConn->GetConnection(GetDbEntry())->SetTableExists();
	
	// Writer Meta-Class Instance
	fParam_Writer = GetParamWriter();
		
	fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"Califa Preamplifier Node Pars");
	
  }

  return fParam_Writer;
}

void R3BCaloPreamplifierPar::update(Int_t seqno)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  R3BCaloPreamplifierPar::Update()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  FairDbString command;
  command << "UPDATE R3BCALOPREAMPLIFIERPARVAL SET TIMEEND = NOW() WHERE SEQNO = " << seqno <<  ";\0";

  stmtDbn->Commit(command.c_str());
  if ( stmtDbn->PrintExceptions() ) {
   fail = true;
   cout << "-E- R3BCaloPreamplifierPar::Update() ******* Error Executing SQL commands ***********  " << endl;
  }

}
