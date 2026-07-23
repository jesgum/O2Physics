// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file flattenAndUploadLutToCCDB.C
/// \brief Flattens and uploads LUTs to ccdb
/// \author ALICE 3 Simulation & Performance group

#include "ALICE3/Core/FlatLutEntry.h"

#include <CCDB/CcdbApi.h>
#include <Framework/Logger.h>

#include <TString.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void flattenAndUploadLutToCCDB()
{
  // Upload path
  const char* ccdbPath = "Users/j/jekarlss/ccdbUploadTest3/luts";

  // Local path to Lookup tables to upload
  const char* lutEl = "lutCovm.el.dat";
  const char* lutMu = "lutCovm.mu.dat";
  const char* lutPi = "lutCovm.pi.dat";
  const char* lutKa = "lutCovm.ka.dat";
  const char* lutPr = "lutCovm.pr.dat";
  const char* lutDe = "";
  const char* lutTr = "";
  const char* lutHe3 = "";
  const char* lutAl = "";

  //___________________________________________________________________________________________________________________________
  std::vector<const char*> subDirectories = {"lutEl", "lutMu", "lutPi", "lutKa", "lutPr", "lutDe", "lutTr", "lutHe3", "lutAl"};
  std::vector<const char*> files = {lutEl, lutMu, lutPi, lutKa, lutPr, lutDe, lutTr, lutHe3, lutAl};
  std::vector<int> pdgs = {11, 13, 211, 321, 2212, 1000010020, 1000010030, 1000020030, 1000020040};
  static constexpr int nLUTs = 9; // Number of LUT available

  o2::ccdb::CcdbApi ccdb;
  ccdb.init("https://alice-ccdb.cern.ch");

  // Go ahead with ccdb uplaod
  std::map<std::string, std::string> metadata; // can be empty
  for (int lut{0}; lut < nLUTs; ++lut) {
    std::ifstream file(files[lut], std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Failed to open file " << files[lut] << "\n";
      continue;
    }

    o2::delphes::FlatLutData lutData = o2::delphes::FlatLutData::loadFromFile(file, files[lut]);
    LOG(info) << "Attempting ccdb upload...";
    try {
      std::string fullPath = Form("%s/%s", ccdbPath, subDirectories[lut]);
      ccdb.storeAsBinaryFile(reinterpret_cast<const char*>(lutData.data()), lutData.bytes(), files[lut], "o2::delphes::FlatLutData", fullPath, metadata, /*tsSOR*/ 1747442464000, /*tsEOR*/ 1747442764000);
    } catch (const std::exception& e) {
      LOG(fatal) << "Failed at CCDB submission!";
    }
    LOG(info) << "Table for pdg " << pdgs[lut] << " uploaded!";
  }
}
