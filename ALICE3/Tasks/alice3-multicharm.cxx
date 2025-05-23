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
//
//  *+-+*+-+*+-+*+-+*+-+*+-+*+-+*+-+*
//   Decay finder task for ALICE 3
//  *+-+*+-+*+-+*+-+*+-+*+-+*+-+*+-+*
//
//    Uses specific ALICE 3 PID and performance for studying
//    HF decays. Work in progress: use at your own risk!
//

#include <cmath>
#include <array>
#include <cstdlib>
#include <map>
#include <iterator>
#include <utility>

#include "Framework/runDataProcessing.h"
#include "Framework/RunningWorkflowInfo.h"
#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"
#include "Framework/ASoAHelpers.h"
#include "DCAFitter/DCAFitterN.h"
#include "ReconstructionDataFormats/Track.h"
#include "Common/Core/RecoDecay.h"
#include "Common/Core/trackUtilities.h"
#include "PWGLF/DataModel/LFStrangenessTables.h"
#include "PWGLF/DataModel/LFParticleIdentification.h"
#include "Common/Core/TrackSelection.h"
#include "Common/DataModel/TrackSelectionTables.h"
#include "DetectorsBase/Propagator.h"
#include "DetectorsBase/GeometryManager.h"
#include "DataFormatsParameters/GRPObject.h"
#include "DataFormatsParameters/GRPMagField.h"
#include "CCDB/BasicCCDBManager.h"
#include "DataFormatsCalibration/MeanVertexObject.h"
#include "ALICE3/DataModel/OTFTOF.h"
#include "ALICE3/DataModel/RICH.h"
#include "ALICE3/DataModel/A3DecayFinderTables.h"
#include "ALICE3/DataModel/OTFStrangeness.h"
#include "ALICE3/DataModel/OTFMulticharm.h"
#include "ALICE3/DataModel/tracksAlice3.h"
#include "DetectorsVertexing/PVertexer.h"
#include "DetectorsVertexing/PVertexerHelpers.h"
#include "CommonConstants/PhysicsConstants.h"


using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

using multicharmtracks = soa::Join<aod::MCharmIndices, aod::MCharmCores>;

struct alice3multicharm
{
  SliceCache cache;

  ConfigurableAxis axisPt{"axisPt", {VARIABLE_WIDTH, 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.0f, 2.2f, 2.4f, 2.6f, 2.8f, 3.0f, 3.2f, 3.4f, 3.6f, 3.8f, 4.0f, 4.4f, 4.8f, 5.2f, 5.6f, 6.0f, 6.5f, 7.0f, 7.5f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 17.0f, 19.0f, 21.0f, 23.0f, 25.0f, 30.0f, 35.0f, 40.0f, 50.0f}, "pt axis for QA histograms"};

  Configurable<float> xiMinDCAxy{"xiMinDCAxy", -1, "[0] in |DCAxy| > [0]+[1]/pT"};
  Configurable<float> xiMinDCAz{"xiMinDCAz", -1, "[0] in |DCAz| > [0]+[1]/pT"};
  Configurable<float> xiMinRadius{"xiMinRadius", -1, "Minimum R2D for XiC decay (cm)"};
  Configurable<float> xiMassWindow{"xiMassWindow", 0.015, "Mass window around Xi peak (GeV/c)"};

  Configurable<float> picMinDCAxy{"picMinDCAxy", -1, "[0] in |DCAz| > [0]+[1]/pT"};
  Configurable<float> picMinDCAz{"picMinDCAz", -1, "[0] in |DCAxy| > [0]+[1]/pT"};
  Configurable<float> picTofDiffInner{"picTofDiffInner", 1e+4, "|signal - expected| (ps)"};
  Configurable<float> picMinPt{"picMinPt", -1, "Minimum pT for XiC pions"};
  
  Configurable<float> piccMinDCAxy{"piccMinDCAxy", -1, "[0] in |DCAxy| > [0]+[1]/pT"};
  Configurable<float> piccMinDCAz{"piccMinDCAz", -1, "[0] in |DCAz| > [0]+[1]/pT"};
  Configurable<float> piccTofDiffInner{"piccTofDiffInner", 1e+4, "|signal - expected| (ps)"};
  Configurable<float> piccMinPt{"piccMinPt", -1, "Minimum pT for XiCC pions"};

  Configurable<float> xicMaxDauDCA{"xicMaxDauDCA", 1e+4, "DCA between XiC daughters (cm)"};
  Configurable<float> xicMinDCAxy{"xicMinDCAxy", -1, "[0] in |DCAz| > [0]+[1]/pT"};
  Configurable<float> xicMinDCAz{"xicMinDCAz", -1, "[0] in |DCAxy| > [0]+[1]/pT"};
  Configurable<float> xiccMaxDCAxy{"xiccMaxDCAxy", 1e+4, "Maximum DCAxy"};
  Configurable<float> xiccMaxDCAz{"xiccMaxDCAz", 1e+4, "Maximum DCAz"};
  Configurable<float> xicMinRadius{"xicMinRadius", -1, "Minimum R2D for XiC decay (cm)"};
  Configurable<float> xicMinDecayDistanceFromPV{"xicMinDecayDistanceFromPV", 1e+4, "Minimum distance for XiC decay from PV (cm)"};
  Configurable<float> xicMinProperLength{"xicMinProperLength", -1, "Minimum proper length for XiC decay (cm)"};
  Configurable<float> xicMaxProperLength{"xicMaxProperLength", 1e+4, "Minimum proper length for XiC decay (cm)"};
  Configurable<float> xicMassWindow{"xicMassWindow", 0.015, "Mass window around XiC peak (GeV/c)"};
  
  Configurable<float> xiccMaxDauDCA{"xiccMaxDauDCA", 1e+4, "DCA between XiCC daughters (cm)"};
  Configurable<float> xiccMinRadius{"xiccMinRadius", -1, "Minimum R2D for XiCC decay (cm)"};
  Configurable<float> xiccMinProperLength{"xiccMinProperLength", -1, "Minimum proper length for XiCC decay (cm)"};
  Configurable<float> xiccMaxProperLength{"xiccMaxProperLength", 1e+4, "Minimum proper length for XiCC decay (cm)"};
  Configurable<float> xiccMaxEta{"xiccMaxEta", 1.5, "Max eta"};

  HistogramRegistry histos{"histos", {}, OutputObjHandlingPolicy::AnalysisObject};

  void init(InitContext&)
  {
    histos.add("hDCAXiCDaughters", "hDCAXiCDaughters", kTH1D, {axisPt});
  }

  void processXicc(multicharmtracks const& multiCharmTracks)
  {
    for (const auto& xiccCand : multiCharmTracks) {
      if (std::fabs(xiccCand.xicDauDCA()) > xicMaxDauDCA || std::fabs(xiccCand.xiccDauDCA()) > xiccMaxDauDCA)
        continue;

      if (std::fabs(xiccCand.xiDCAxy()) < xiMinDCAxy || std::fabs(xiccCand.xiDCAz()) < xiMinDCAz)
        continue;

      if (std::fabs(xiccCand.pi1cDCAxy()) < picMinDCAxy || std::fabs(xiccCand.pi1cDCAz()) < picMinDCAz)
        continue;

      if (std::fabs(xiccCand.pi2cDCAxy()) < picMinDCAxy || std::fabs(xiccCand.pi2cDCAz()) < picMinDCAz)
        continue;

      if (std::fabs(xiccCand.piccDCAxy()) < piccMinDCAxy || std::fabs(xiccCand.piccDCAz()) < piccMinDCAz)
        continue;
      
      if (std::fabs(xiccCand.xicDCAxy()) < xicMinDCAxy || std::fabs(xiccCand.xicDCAz()) < xicMinDCAz)
        continue;

      if (std::fabs(xiccCand.pi1cDCAxy()) < picMinDCAxy || std::fabs(xiccCand.pi1cDCAz()) < picMinDCAz)
        continue;

      if (std::fabs(xiccCand.pi2cDCAxy()) < picMinDCAxy || std::fabs(xiccCand.pi2cDCAz()) < picMinDCAz)
        continue;

      if (std::fabs(xiccCand.xiccDCAxy()) > xiccMaxDCAxy || std::fabs(xiccCand.xiccDCAz()) > xiccMaxDCAz)
        continue;


        // picTofDiffInner
        // piccTofDiffInner
        // xiccMaxDCAxy
        // xiccMaxDCAz
        // picMinPt
        // piccMinPt
        // xiMinRadius
        // xicMinRadius
        // xiccMinRadius

    }
  }

  PROCESS_SWITCH(alice3multicharm, processXicc, "find XiCC baryons", true);

};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<alice3multicharm>(cfgc)};
}
