#include "common/stats/scope_prefixer.h"

#include "envoy/stats/scope.h"

#include "common/stats/symbol_table_impl.h"
#include "common/stats/utility.h"

namespace Envoy {
namespace Stats {

ScopePrefixer::ScopePrefixer(absl::string_view prefix, Scope& scope)
    : scope_(scope), prefix_(Utility::sanitizeStatsName(prefix), symbolTable()) {}

ScopePrefixer::ScopePrefixer(StatName prefix, Scope& scope)
    : scope_(scope), prefix_(prefix, symbolTable()) {}

ScopePrefixer::~ScopePrefixer() { prefix_.free(symbolTable()); }

ScopePtr ScopePrefixer::createScopeFromStatName(StatName name) {
  SymbolTable::StoragePtr joined = symbolTable().join({prefix_.statName(), name});
  return std::make_unique<ScopePrefixer>(StatName(joined.get()), scope_);
}

ScopePtr ScopePrefixer::createScope(const std::string& name) {
  StatNameManagedStorage stat_name_storage(Utility::sanitizeStatsName(name), symbolTable());
  return createScopeFromStatName(stat_name_storage.statName());
}

Counter& ScopePrefixer::counterFromStatName(StatName name) {
  Stats::SymbolTable::StoragePtr stat_name_storage =
      scope_.symbolTable().join({prefix_.statName(), name});
  return scope_.counterFromStatName(StatName(stat_name_storage.get()));
}

Gauge& ScopePrefixer::gaugeFromStatName(StatName name) {
  Stats::SymbolTable::StoragePtr stat_name_storage =
      scope_.symbolTable().join({prefix_.statName(), name});
  return scope_.gaugeFromStatName(StatName(stat_name_storage.get()));
}

Histogram& ScopePrefixer::histogramFromStatName(StatName name) {
  Stats::SymbolTable::StoragePtr stat_name_storage =
      scope_.symbolTable().join({prefix_.statName(), name});
  return scope_.histogramFromStatName(StatName(stat_name_storage.get()));
}

absl::optional<std::reference_wrapper<const Counter>>
ScopePrefixer::findCounter(StatName name) const {
  Stats::SymbolTable::StoragePtr stat_name_storage =
      scope_.symbolTable().join({prefix_.statName(), name});
  return scope_.findCounter(StatName(stat_name_storage.get()));
}

absl::optional<std::reference_wrapper<const Gauge>> ScopePrefixer::findGauge(StatName name) const {
  Stats::SymbolTable::StoragePtr stat_name_storage =
      scope_.symbolTable().join({prefix_.statName(), name});
  return scope_.findGauge(StatName(stat_name_storage.get()));
}

absl::optional<std::reference_wrapper<const Histogram>>
ScopePrefixer::findHistogram(StatName name) const {
  Stats::SymbolTable::StoragePtr stat_name_storage =
      scope_.symbolTable().join({prefix_.statName(), name});
  return scope_.findHistogram(StatName(stat_name_storage.get()));
}

void ScopePrefixer::deliverHistogramToSinks(const Histogram& histograms, uint64_t val) {
  scope_.deliverHistogramToSinks(histograms, val);
}

} // namespace Stats
} // namespace Envoy
