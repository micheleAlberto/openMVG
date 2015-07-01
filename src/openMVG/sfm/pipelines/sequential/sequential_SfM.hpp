
// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "openMVG/sfm/pipelines/sfm_engine.hpp"
#include "openMVG/sfm/pipelines/sfm_features_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_matches_provider.hpp"
#include "openMVG/tracks/tracks.hpp"

#include "third_party/htmlDoc/htmlDoc.hpp"
#include "third_party/histogram/histogram.hpp"

namespace openMVG {
namespace sfm {

/// Sequential SfM Pipeline Reconstruction Engine.
class SequentialSfMReconstructionEngine : public ReconstructionEngine
{
public:

  SequentialSfMReconstructionEngine(
    const SfM_Data & sfm_data,
    const std::string & soutDirectory,
    const std::string & loggingFile = "",
    const std::string & trackFileName = "");

  ~SequentialSfMReconstructionEngine();

  void SetFeaturesProvider(Features_Provider * provider);
  void SetMatchesProvider(Matches_Provider * provider);

  virtual bool Process();

  void setInitialPair(const Pair & initialPair)
  {
    _initialpair = initialPair;
  }

  /// Initialize tracks
  bool InitLandmarkTracks();
  bool InitLandmarkTracks(std::string filename);
  /// Select a candidate initial pair
  bool ChooseInitialPair(Pair & initialPairIndex) const;

  /// Compute the initial 3D seed (First camera t=0; R=Id, second estimated by 5 point algorithm)
  bool MakeInitialPair3D(const Pair & initialPair);

  void SetUnknownCameraType(const cameras::EINTRINSIC camType)
  {
    _camType = camType;
  }

protected:


private:

  /// Return MSE (Mean Square Error) and a histogram of residual values.
  double ComputeResidualsHistogram(Histogram<double> * histo);

  /// List the images that the greatest number of matches to the current 3D reconstruction.
  bool FindImagesWithPossibleResection(std::vector<size_t> & vec_possible_indexes);

  /// Add to the current scene the desired image indexes.
  bool Resection(std::vector<size_t> & vec_possible_indexes);

  /// Add a single Image to the scene and triangulate new possible tracks.
  bool Resection(size_t imageIndex);

  /// Bundle adjustment to refine Structure; Motion and Intrinsics
  void BundleAdjustment();

  /// Discard track with too large residual error
  size_t badTrackRejector(double dPrecision);

  //----
  //-- Data
  //----

  // HTML logger
  std::shared_ptr<htmlDocument::htmlDocumentStream> _htmlDocStream;
  std::string _sLoggingFile;

  // Parameter
  Pair _initialpair;
  cameras::EINTRINSIC _camType; // The camera type for the unknown cameras
  std::string _trackFileName;
  //-- Data provider
  Features_Provider  * _features_provider;
  Matches_Provider  * _matches_provider;

  // Temporary data
  openMVG::tracks::STLMAPTracks _map_tracks; // putative landmark tracks (visibility per 3D point)
  Hash_Map<IndexT, double> _map_ACThreshold; // Per camera confidence (A contrario estimated threshold error)

  std::set<size_t> _set_remainingViewId;     // Remaining camera index that can be used for resection
};

} // namespace sfm
} // namespace openMVG

