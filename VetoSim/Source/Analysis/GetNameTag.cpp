void GetNameTag()
{
  // This function obtains the NameTag from
  // the inputs.root file and writes it to a .txt-file
  // so that the shell scripts have access to this string.
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the NameTag:
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  TString Output_Name = Output_Name_Obj->GetString();
  
  // convert it to a C++ string:
  string str = (TString) Output_Name;
  
  // Now create a .txt file with C++ code alone:
  ofstream txt_file = ("../../Data/Inputs/NameTag.txt");
  
  // Now write n to the .txt file:
  txt_file << str;
  
  // close the txt file:
  txt_file.close();
  
  // Done!
}
 