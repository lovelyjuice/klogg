/*
 * Copyright (C) 2009, 2010, 2011, 2013, 2015 Nicolas Bonnefon and other contributors
 *
 * This file is part of glogg.
 *
 * glogg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glogg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glogg.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QFont>
#include <QSettings>

#include "persistable.h"

// Type of regexp to use for searches
enum SearchRegexpType {
    ExtendedRegexp,
    Wildcard,           // Disused!
    FixedString,
};

// Configuration class containing everything in the "Settings" dialog
class Configuration : public Persistable {
  public:
    Configuration();

    // Accesses the main font used for display
    QFont mainFont() const;
    void setMainFont( QFont newFont );

    // Accesses the regexp types
    SearchRegexpType mainRegexpType() const
    { return mainRegexpType_; }
    SearchRegexpType quickfindRegexpType() const
    { return quickfindRegexpType_; }
    bool isQuickfindIncremental() const
    { return quickfindIncremental_; }
    void setMainRegexpType( SearchRegexpType type )
    { mainRegexpType_ = type; }
    void setQuickfindRegexpType( SearchRegexpType type )
    { quickfindRegexpType_ = type; }
    void setQuickfindIncremental( bool is_incremental )
    { quickfindIncremental_ = is_incremental; }

    // "Advanced" settings
    bool pollingEnabled() const
    { return pollingEnabled_; }
    void setPollingEnabled( bool enabled )
    { pollingEnabled_ = enabled; }
    uint32_t pollIntervalMs() const
    { return pollIntervalMs_; }
    void setPollIntervalMs( uint32_t interval )
    { pollIntervalMs_ = interval; }
    bool loadLastSession() const
    { return loadLastSession_; }
    void setLoadLastSession( bool enabled )
    { loadLastSession_ = enabled; }

    // perf settings
    bool useParallelSearch() const
    { return useParallelSearch_; }
    void setUseParallelSearch( bool enabled )
    { useParallelSearch_ = enabled; }
    bool useSearchResultsCache() const
    { return useSearchResultsCache_; }
    void setUseSearchResultsCache( bool enabled )
    { useSearchResultsCache_ = enabled; }
    uint32_t searchResultsCacheLines() const
    { return searchResultsCacheLines_; }
    void setSearchResultsCacheLines( uint32_t lines )
    { searchResultsCacheLines_ = lines; }
    uint32_t indexReadBufferSizeMb() const
    { return indexReadBufferSizeMb_; }
    void setIndexReadBufferSizeMb( uint32_t bufferSizeMb )
    { indexReadBufferSizeMb_ = bufferSizeMb; }
    uint32_t searchReadBufferSizeLines() const
    { return searchReadBufferSizeLines_; }
    void setSearchReadBufferSizeLines( uint32_t lines )
    { searchReadBufferSizeLines_ = lines; }

    // View settings
    bool isOverviewVisible() const
    { return overviewVisible_; }
    void setOverviewVisible( bool isVisible )
    { overviewVisible_ = isVisible; }
    bool mainLineNumbersVisible() const
    { return lineNumbersVisibleInMain_; }
    bool filteredLineNumbersVisible() const
    { return lineNumbersVisibleInFiltered_; }
    void setMainLineNumbersVisible( bool lineNumbersVisible )
    { lineNumbersVisibleInMain_ = lineNumbersVisible; }
    void setFilteredLineNumbersVisible( bool lineNumbersVisible )
    { lineNumbersVisibleInFiltered_ = lineNumbersVisible; }

    // Default settings for new views
    bool isSearchAutoRefreshDefault() const
    { return searchAutoRefresh_; }
    void setSearchAutoRefreshDefault( bool auto_refresh )
    { searchAutoRefresh_ = auto_refresh; }
    bool isSearchIgnoreCaseDefault() const
    { return searchIgnoreCase_; }
    void setSearchIgnoreCaseDefault( bool ignore_case )
    { searchIgnoreCase_ = ignore_case; }
    QList<int> splitterSizes() const
    { return splitterSizes_; }
    void setSplitterSizes( QList<int> sizes )
    { splitterSizes_ = std::move( sizes ); }

    // Reads/writes the current config in the QSettings object passed
    void saveToStorage( QSettings& settings ) const override;
    void retrieveFromStorage( QSettings& settings ) override;

  private:
    // Configuration settings
    QFont mainFont_;
    SearchRegexpType mainRegexpType_;
    SearchRegexpType quickfindRegexpType_;
    bool quickfindIncremental_;
    bool pollingEnabled_;
    uint32_t pollIntervalMs_;
    bool loadLastSession_;

    // View settings
    bool overviewVisible_;
    bool lineNumbersVisibleInMain_;
    bool lineNumbersVisibleInFiltered_;

    // Default settings for new views
    bool searchAutoRefresh_;
    bool searchIgnoreCase_;
    QList<int> splitterSizes_;

    // Performance settings
    bool useSearchResultsCache_;
    uint32_t searchResultsCacheLines_;
    bool useParallelSearch_;
    uint32_t indexReadBufferSizeMb_;
    uint32_t searchReadBufferSizeLines_;

};

#endif
