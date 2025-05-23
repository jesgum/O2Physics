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

///
/// \file   OTFStrangeness.h
/// \author David Dobrigkeit Chinellato
/// \since  05/08/2024
/// \brief  Set of tables for the ALICE3 strangeness information
///

#ifndef ALICE3_DATAMODEL_OTFMULTICHARM_H_
#define ALICE3_DATAMODEL_OTFMULTICHARM_H_

// O2 includes
#include "Framework/AnalysisDataModel.h"

namespace o2::aod
{
namespace otfmulticharm
{
DECLARE_SOA_INDEX_COLUMN_FULL(Cascade, cascade, int, UpgradeCascades, "_Cascade");
DECLARE_SOA_INDEX_COLUMN_FULL(XiCPion1, xiCPion1, int, Tracks, "_Pi1XiC");
DECLARE_SOA_INDEX_COLUMN_FULL(XiCPion2, xiCPion2, int, Tracks, "_Pi2XiC");
DECLARE_SOA_INDEX_COLUMN_FULL(XiCCPion, xiCCPion, int, Tracks, "_PiXiCC");

// topo vars xicDaughtersDCA
DECLARE_SOA_COLUMN(XicDauDCA, xicDauDCA, float);
DECLARE_SOA_COLUMN(XiccDauDCA, xiccDauDCA, float);

DECLARE_SOA_COLUMN(MXiC, mXic, float);
DECLARE_SOA_COLUMN(MXiCC, mXicc, float);

// kine vars
DECLARE_SOA_COLUMN(Pt, pt, float);
DECLARE_SOA_COLUMN(Eta, eta, float);

// tracking counters
DECLARE_SOA_COLUMN(NSiliconHitsXi, nSiliconHitsXi, int);
DECLARE_SOA_COLUMN(NSiliconHitsPiFromXi, nSiliconHitsPiFromXi, int);
DECLARE_SOA_COLUMN(NSiliconHitsPiFromLa, nSiliconHitsPiFromLa, int);
DECLARE_SOA_COLUMN(NSiliconHitsPrFromLa, nSiliconHitsPrFromLa, int);
DECLARE_SOA_COLUMN(NSiliconHitsPiC1, nSiliconHitsPiC1, int);
DECLARE_SOA_COLUMN(NSiliconHitsPiC2, nSiliconHitsPiC2, int);
DECLARE_SOA_COLUMN(NSiliconHitsPiCC, nSiliconHitsPiCC, int);

DECLARE_SOA_COLUMN(NTPCHitsPiFromXi, nTPCHitsPiFromXi, int);
DECLARE_SOA_COLUMN(NTPCHitsPiFromLa, nTPCHitsPiFromLa, int);
DECLARE_SOA_COLUMN(NTPCHitsPrFromLa, nTPCHitsPrFromLa, int);
DECLARE_SOA_COLUMN(NTPCHitsPiC1, nTPCHitsPiC1, int);
DECLARE_SOA_COLUMN(NTPCHitsPiC2, nTPCHitsPiC2, int);
DECLARE_SOA_COLUMN(NTPCHitsPiCC, nTPCHitsPiCC, int);

// DCA to PV variables
DECLARE_SOA_COLUMN(XiDCAxy, xiDCAxy, float);
DECLARE_SOA_COLUMN(XicDCAxy, xicDCAxy, float);
DECLARE_SOA_COLUMN(XiccDCAxy, xiccDCAxy, float);
DECLARE_SOA_COLUMN(XiDCAz, xiDCAz, float);
DECLARE_SOA_COLUMN(XicDCAz, xicDCAz, float);
DECLARE_SOA_COLUMN(XiccDCAz, xiccDCAz, float);

DECLARE_SOA_COLUMN(PiFromXiDCAxy, piFromXiDCAxy, float);
DECLARE_SOA_COLUMN(PiFromLaDCAxy, piFromLaDCAxy, float);
DECLARE_SOA_COLUMN(PrFromLaDCAxy, prFromLaDCAxy, float);
DECLARE_SOA_COLUMN(PiFromXiDCAz, piFromXiDCAz, float);
DECLARE_SOA_COLUMN(PiFromLaDCAz, piFromLaDCAz, float);
DECLARE_SOA_COLUMN(PrFromLaDCAz, prFromLaDCAz, float);

DECLARE_SOA_COLUMN(Pi1cDCAxy, pi1cDCAxy, float);
DECLARE_SOA_COLUMN(Pi2cDCAxy, pi2cDCAxy, float);
DECLARE_SOA_COLUMN(PiccDCAxy, piccDCAxy, float);
DECLARE_SOA_COLUMN(Pi1cDCAz, pi1cDCAz, float);
DECLARE_SOA_COLUMN(Pi2cDCAz, pi2cDCAz, float);
DECLARE_SOA_COLUMN(PiccDCAz, piccDCAz, float);

// Lengths
DECLARE_SOA_COLUMN(XicDecayRadius2D, xicDecayRadius2D, float);
DECLARE_SOA_COLUMN(XiccDecayRadius2D, xiccDecayRadius2D, float);
DECLARE_SOA_COLUMN(XicProperLength, xicProperLength, float);
DECLARE_SOA_COLUMN(XicDistanceFromPV, xicDistanceFromPV, float);
DECLARE_SOA_COLUMN(XiccProperLength, xiccProperLength, float);

} // namespace otfmulticharm
DECLARE_SOA_TABLE(MCharmIndices, "AOD", "MCharmIndices",
                  o2::soa::Index<>,
                  otfmulticharm::CascadeId,
                  otfmulticharm::XiCPion1Id,
                  otfmulticharm::XiCPion2Id,
                  otfmulticharm::XiCCPionId);

DECLARE_SOA_TABLE(MCharmCores, "AOD", "MCharmCores",
                  otfmulticharm::XicDauDCA,
                  otfmulticharm::XiccDauDCA,
                  otfmulticharm::MXiC,
                  otfmulticharm::MXiCC,
                  otfmulticharm::Pt,
                  otfmulticharm::Eta,

                  otfmulticharm::NSiliconHitsXi,
                  otfmulticharm::NSiliconHitsPiFromXi,
                  otfmulticharm::NSiliconHitsPiFromLa,
                  otfmulticharm::NSiliconHitsPrFromLa,
                  otfmulticharm::NSiliconHitsPiC1,
                  otfmulticharm::NSiliconHitsPiC2,
                  otfmulticharm::NSiliconHitsPiCC,
                  otfmulticharm::NTPCHitsPiFromXi,
                  otfmulticharm::NTPCHitsPiFromLa,
                  otfmulticharm::NTPCHitsPrFromLa,
                  otfmulticharm::NTPCHitsPiC1,
                  otfmulticharm::NTPCHitsPiC2,
                  otfmulticharm::NTPCHitsPiCC,

                  otfmulticharm::XiDCAxy,
                  otfmulticharm::XicDCAxy,
                  otfmulticharm::XiccDCAxy,
                  otfmulticharm::XiDCAz,
                  otfmulticharm::XicDCAz,
                  otfmulticharm::XiccDCAz,

                  otfmulticharm::PiFromXiDCAxy,
                  otfmulticharm::PiFromLaDCAxy,
                  otfmulticharm::PrFromLaDCAxy,
                  otfmulticharm::PiFromXiDCAz,
                  otfmulticharm::PiFromLaDCAz,
                  otfmulticharm::PrFromLaDCAz,

                  otfmulticharm::Pi1cDCAxy,
                  otfmulticharm::Pi2cDCAxy,
                  otfmulticharm::PiccDCAxy,
                  otfmulticharm::Pi1cDCAz,
                  otfmulticharm::Pi2cDCAz,
                  otfmulticharm::PiccDCAz,

                  otfmulticharm::XicDecayRadius2D,
                  otfmulticharm::XiccDecayRadius2D,
                  otfmulticharm::XicProperLength,
                  otfmulticharm::XicDistanceFromPV,
                  otfmulticharm::XiccProperLength);

} // namespace o2::aod

#endif // ALICE3_DATAMODEL_OTFMULTICHARM_H_
