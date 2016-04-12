// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2016, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================
// Interface functions for banded local alignment.
// ==========================================================================

#ifndef SEQAN_INCLUDE_SEQAN_ALIGN_LOCAL_ALIGNMENT_BANDED_H_
#define SEQAN_INCLUDE_SEQAN_ALIGN_LOCAL_ALIGNMENT_BANDED_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function localAlignment()                                    [banded, Align]
// ----------------------------------------------------------------------------

template <typename TSequence, typename TAlignSpec, typename TScoreValue, typename TScoreSpec, typename TTag>
TScoreValue localAlignment(Align<TSequence, TAlignSpec> & align,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag,
                           TTag const & tag)
{
    typedef Align<TSequence, TAlignSpec> TAlign;
    typedef typename Size<TAlign>::Type TSize;
    typedef typename Position<TAlign>::Type TPosition;
    typedef TraceSegment_<TPosition, TSize> TTraceSegment;
    typedef AlignConfig2<DPLocal, DPBandConfig<BandOn>, FreeEndGaps_<> > TAlignConfig2;

    SEQAN_ASSERT_EQ(length(rows(align)), 2u);

    String<TTraceSegment> trace;
    DPScoutState_<Default> dpScoutState;
    TScoreValue res = _setUpAndRunAlignment(trace, dpScoutState, source(row(align, 0)), source(row(align, 1)),
                                            scoringScheme, TAlignConfig2(lowerDiag, upperDiag), tag);

    _adaptTraceSegmentsTo(row(align, 0), row(align, 1), trace);
    return res;
}

template <typename TSequence, typename TAlignSpec, typename TScoreValue, typename TScoreSpec>
TScoreValue localAlignment(Align<TSequence, TAlignSpec> & align,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag)
{
    SEQAN_ASSERT(length(rows(align)) == 2u);
    if (_usesAffineGaps(scoringScheme, source(row(align, 0)), source(row(align, 1))))
        return localAlignment(align, scoringScheme, lowerDiag, upperDiag, AffineGaps());
    else
        return localAlignment(align, scoringScheme, lowerDiag, upperDiag, LinearGaps());
}

// ----------------------------------------------------------------------------
// Function localAlignment()                                     [banded, Gaps]
// ----------------------------------------------------------------------------

template <typename TSequenceH, typename TGapsSpecH, typename TSequenceV, typename TGapsSpecV,
          typename TScoreValue, typename TScoreSpec, typename TTag>
TScoreValue localAlignment(Gaps<TSequenceH, TGapsSpecH> & gapsH,
                           Gaps<TSequenceV, TGapsSpecV> & gapsV,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag,
                           TTag const & tag)
{
    typedef typename Size<TSequenceH>::Type TSize;
    typedef typename Position<TSequenceH>::Type TPosition;
    typedef TraceSegment_<TPosition, TSize> TTraceSegment;
    typedef AlignConfig2<DPLocal, DPBandConfig<BandOn>, FreeEndGaps_<> > TAlignConfig2;

    String<TTraceSegment> trace;
    DPScoutState_<Default> dpScoutState;
    TScoreValue res = _setUpAndRunAlignment(trace, dpScoutState, source(gapsH), source(gapsV), scoringScheme,
                                            TAlignConfig2(lowerDiag, upperDiag), tag);
    _adaptTraceSegmentsTo(gapsH, gapsV, trace);
    return res;
}

template <typename TSequenceH, typename TGapsSpecH, typename TSequenceV, typename TGapsSpecV,
          typename TScoreValue, typename TScoreSpec>
TScoreValue localAlignment(Gaps<TSequenceH, TGapsSpecH> & gapsH,
                           Gaps<TSequenceV, TGapsSpecV> & gapsV,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag)
{
    if (_usesAffineGaps(scoringScheme, source(gapsH), source(gapsV)))
        return localAlignment(gapsH, gapsV, scoringScheme, lowerDiag, upperDiag, AffineGaps());
    else
        return localAlignment(gapsH, gapsV, scoringScheme, lowerDiag, upperDiag, LinearGaps());
}

// ----------------------------------------------------------------------------
// Function localAlignment()                      [banded, Graph<Alignment<> >]
// ----------------------------------------------------------------------------

template <typename TStringSet, typename TCargo, typename TGraphSpec, typename TScoreValue, typename TScoreSpec,
          typename TTag>
TScoreValue localAlignment(Graph<Alignment<TStringSet, TCargo, TGraphSpec> > & alignmentGraph,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag,
                           TTag const & tag)
{
    typedef Graph<Alignment<TStringSet, TCargo, TGraphSpec> > TGraph;
    typedef typename Size<TGraph>::Type TSize;
    typedef typename Position<TGraph>::Type TPosition;
    typedef TraceSegment_<TPosition, TSize> TTraceSegment;
    typedef AlignConfig2<DPLocal, DPBandConfig<BandOn>, FreeEndGaps_<> > TAlignConfig2;

    String<TTraceSegment> trace;
    DPScoutState_<Default> dpScoutState;
    TScoreValue res = _setUpAndRunAlignment(trace, dpScoutState, value(stringSet(alignmentGraph), 0),
                                            value(stringSet(alignmentGraph), 1), scoringScheme,
                                            TAlignConfig2(lowerDiag, upperDiag), tag);

    _adaptTraceSegmentsTo(alignmentGraph, positionToId(stringSet(alignmentGraph), 0),
                          positionToId(stringSet(alignmentGraph), 1), trace);
    return res;
}

template <typename TStringSet, typename TCargo, typename TGraphSpec,
          typename TScoreValue, typename TScoreSpec>
TScoreValue localAlignment(Graph<Alignment<TStringSet, TCargo, TGraphSpec> > & alignmentGraph,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag)
{
    SEQAN_ASSERT(length(stringSet(alignmentGraph)) == 2u);

    if (_usesAffineGaps(scoringScheme, stringSet(alignmentGraph)[0], stringSet(alignmentGraph)[1]))
        return localAlignment(alignmentGraph, scoringScheme, lowerDiag, upperDiag, AffineGaps());
    else
        return localAlignment(alignmentGraph, scoringScheme, lowerDiag, upperDiag, LinearGaps());
}

// ----------------------------------------------------------------------------
// Function localAlignment()                      [banded, String<Fragment<> >]
// ----------------------------------------------------------------------------

// Full interface.
template <typename TSize, typename TFragmentSpec, typename TStringSpec, typename TSequence, typename TStringSetSpec,
          typename TScoreValue, typename TScoreSpec, typename TTag>
TScoreValue localAlignment(String<Fragment<TSize, TFragmentSpec>, TStringSpec> & fragmentString,
                           StringSet<TSequence, TStringSetSpec> const & strings,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag,
                           TTag const & tag)
{
    typedef String<Fragment<TSize, TFragmentSpec>, TStringSpec> TFragments;
    typedef typename Position<TFragments>::Type TPosition;
    typedef TraceSegment_<TPosition, TSize> TTraceSegment;
    typedef AlignConfig2<DPLocal, DPBandConfig<BandOn>, FreeEndGaps_<> > TAlignConfig2;

    String<TTraceSegment> trace;
    DPScoutState_<Default> dpScoutState;
    TScoreValue res = _setUpAndRunAlignment(trace, dpScoutState, value(strings, 0), value(strings, 1), scoringScheme,
                                            TAlignConfig2(lowerDiag, upperDiag), tag);
    _adaptTraceSegmentsTo(fragmentString, positionToId(strings, 0), positionToId(strings, 1), trace);
    return res;
}

template <typename TSize, typename TFragmentSpec, typename TStringSpec,
          typename TSequence, typename TStringSetSpec,
          typename TScoreValue, typename TScoreSpec>
TScoreValue localAlignment(String<Fragment<TSize, TFragmentSpec>, TStringSpec> & fragmentString,
                           StringSet<TSequence, TStringSetSpec> const & strings,
                           Score<TScoreValue, TScoreSpec> const & scoringScheme,
                           int lowerDiag,
                           int upperDiag)
{
    SEQAN_ASSERT(length(strings) == 2u);

    if (_usesAffineGaps(scoringScheme, strings[0], strings[1]))
        return localAlignment(fragmentString, strings, scoringScheme, lowerDiag, upperDiag, AffineGaps());
    else
        return localAlignment(fragmentString, strings, scoringScheme, lowerDiag, upperDiag, LinearGaps());
}

// ----------------------------------------------------------------------------
// Function localAlignment()         [banded, SIMD version, StringSet<Align>]
// ----------------------------------------------------------------------------

template <typename TSequence, typename TAlignSpec, typename TScoreValue, typename TScoreSpec, typename TTag>
String<TScoreValue> localAlignment(StringSet<Align<TSequence, TAlignSpec> > & align,
                                   Score<TScoreValue, TScoreSpec> const & scoringScheme,
                                   int lowerDiag, int upperDiag,
                                   TTag const & tag)
{
    typedef Align<TSequence, TAlignSpec> TAlign;
    typedef typename Size<TAlign>::Type TSize;
    typedef typename Position<TAlign>::Type TPosition;
    typedef TraceSegment_<TPosition, TSize> TTraceSegment;
    typedef AlignConfig2<DPLocal, DPBandConfig<BandOn>, FreeEndGaps_<> > TAlignConfig2;
    typedef AlignConfig2<DPLocal, DPBandConfig<BandOff>, FreeEndGaps_<> > TAlignConfig3;

    // create a SIMD scoring scheme
    Score<TSimdAlign, TScoreSpec> scoringSchemeSimd = _setSimdScoringScheme(scoringScheme, TSimdAlign());

    size_t const numAlignments = length(align);
    size_t const sizeBatch = LENGTH<TSimdAlign>::VALUE;

    String<TScoreValue> results;
    resize(results, numAlignments);

    //iterate over alignments with a batch size of sizeBatch
    TSimdAlign resultsBatch;
    for(size_t pos = 0; pos < numAlignments/sizeBatch; ++pos)
    {
        StringSet<String<TTraceSegment> > trace;
        resize(trace, sizeBatch);
        String<TSimdAlign> stringSimdH, stringSimdV, masksH, masksV, masks;
        std::vector<size_t> endsH, endsV;

        // create the SIMD representation of the alignments
        // in case of a variable length alignment the variables masks, endsH, endsV will be filled
        _checkAndCreateSimdRepresentation(align, pos*sizeBatch,
                                          stringSimdH, stringSimdV,
                                          masksH, masksV, masks, endsH, endsV);

        // if alignments have equal dimensions do nothing
        if(endsH.size() == 0)
        {
            DPScoutState_<SimdAlignmentScoutDefault> dpScoutState;
            resultsBatch = _setUpAndRunAlignment(trace, dpScoutState, stringSimdH, stringSimdV,
                                                 scoringSchemeSimd, TAlignConfig2(lowerDiag, upperDiag), tag);
        }
        // otherwise prepare the special DPScoutState
        else
        {
            DPScoutState_<SimdAlignmentScoutVariable> dpScoutState;
            dpScoutState.dimV = length(stringSimdV);
            dpScoutState.isLocalAlignment = true;
            dpScoutState.RIGHT = false;
            dpScoutState.BOTTOM = false;
            swap(dpScoutState.masksH, masksH);
            swap(dpScoutState.masksV, masksV);
            swap(dpScoutState.masks, masks);
            std::swap(dpScoutState.endsH, endsH);
            std::swap(dpScoutState.endsV, endsV);
            // band parameters are ignored for now!
            resultsBatch = _setUpAndRunAlignment(trace, dpScoutState, stringSimdH, stringSimdV,
                                                 scoringSchemeSimd, TAlignConfig3(), tag);
        }

        // copy results and finish traceback
        for(size_t x = pos*sizeBatch; x < (pos+1)*sizeBatch; ++x)
        {
            results[x] = resultsBatch[x-pos*sizeBatch];
            _adaptTraceSegmentsTo(row(align[x], 0), row(align[x], 1), trace[x-pos*sizeBatch]);
        }
    }

    //call the normal non-simd function for remaining alignments
    for(size_t pos = (numAlignments/sizeBatch)*sizeBatch; pos < numAlignments; ++pos)
        results[pos] = localAlignment(align[pos], scoringScheme, lowerDiag, upperDiag);

    return results;
}

template <typename TSequence, typename TAlignSpec,
          typename TScoreValue, typename TScoreSpec>
String<TScoreValue> localAlignment(StringSet<Align<TSequence, TAlignSpec> > & align,
                                   Score<TScoreValue, TScoreSpec> const & scoringScheme,
                                   int lowerDiag, int upperDiag)
{
   if (_usesAffineGaps(scoringScheme, source(row(align[0], 0)), source(row(align[0], 1))))
        return localAlignment(align, scoringScheme, lowerDiag, upperDiag, AffineGaps());
   else
        return localAlignment(align, scoringScheme, lowerDiag, upperDiag, LinearGaps());
}

}  // namespace seqan

#endif  // #ifndef SEQAN_INCLUDE_SEQAN_ALIGN_LOCAL_ALIGNMENT_BANDED_H_
