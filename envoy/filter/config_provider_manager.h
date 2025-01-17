#pragma once

#include "envoy/config/core/v3/config_source.pb.h"
#include "envoy/config/extension_config_provider.h"
#include "envoy/init/manager.h"
#include "envoy/server/filter_config.h"

#include "absl/types/optional.h"

namespace Envoy {
namespace Filter {

template <class FactoryCb>
using FilterConfigProvider = Envoy::Config::ExtensionConfigProvider<FactoryCb>;
template <class FactoryCb>
using FilterConfigProviderPtr = std::unique_ptr<FilterConfigProvider<FactoryCb>>;
template <class FactoryCb>
using DynamicFilterConfigProvider = Envoy::Config::DynamicExtensionConfigProvider<FactoryCb>;
template <class FactoryCb>
using DynamicFilterConfigProviderPtr = std::unique_ptr<DynamicFilterConfigProvider<FactoryCb>>;

/**
 * The FilterConfigProviderManager exposes the ability to get an FilterConfigProvider
 * for both static and dynamic filter config providers.
 */
template <class FactoryCb, class FactoryCtx> class FilterConfigProviderManager {
public:
  virtual ~FilterConfigProviderManager() = default;

  /**
   * Get an FilterConfigProviderPtr for a filter config. The config providers may share
   * the underlying subscriptions to the filter config discovery service.
   * @param config_source supplies the extension configuration source for the filter configs.
   * @param filter_config_name the filter config resource name.
   * @param factory_context is the context to use for the filter config provider.
   * @param stat_prefix supplies the stat_prefix to use for the provider stats.
   * @param last_filter_in_filter_chain indicates whether this filter is the last filter in the
   * configured chain
   * @param filter_chain_type is the filter chain type
   */
  virtual DynamicFilterConfigProviderPtr<FactoryCb> createDynamicFilterConfigProvider(
      const envoy::config::core::v3::ExtensionConfigSource& config_source,
      const std::string& filter_config_name, FactoryCtx& factory_context,
      const std::string& stat_prefix, bool last_filter_in_filter_chain,
      const std::string& filter_chain_type) PURE;

  /**
   * Get an FilterConfigProviderPtr for a statically inlined filter config.
   * @param config is a fully resolved filter instantiation factory.
   * @param filter_config_name is the name of the filter configuration resource.
   */
  virtual FilterConfigProviderPtr<FactoryCb>
  createStaticFilterConfigProvider(const FactoryCb& config,
                                   const std::string& filter_config_name) PURE;

  /**
   * Get the stat prefix for the scope of the filter provider manager.
   */
  virtual absl::string_view statPrefix() const PURE;
};

} // namespace Filter
} // namespace Envoy
