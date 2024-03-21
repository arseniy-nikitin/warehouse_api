#pragma once

#include <handlers/warehouse_client.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/components/component.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>

namespace warehouse_api {

class WarehouseClient final : public userver::components::LoggableComponentBase {
 public:
  WarehouseClient(const userver::components::ComponentConfig& config,
              	  const userver::components::ComponentContext& component_context)
      : userver::components::LoggableComponentBase(config, component_context),
        client_factory_(
            component_context
                .FindComponent<userver::ugrpc::client::ClientFactoryComponent>()
                .GetFactory()),
        // The client needs a fixed endpoint
        client_(client_factory_.MakeClient<handlers::api::WarehouseServiceClient>(
            "warehouse", config["clients"]["warehouse"]["endpoint"].As<std::string>())) {}
  
  int WarehouseClient::CreateWarehouse(std::string address, std::string area);
  warehouse::Warehouse WarehouseClient::GetWarehouse(int id);
  std::vector<warehouse::Warehouse> WarehouseClient::ListWarehouses(int offset, int limit);

  
  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::ugrpc::client::ClientFactory& client_factory_;
  handlers::api::HelloServiceClient client_;
};

void AppendWarehouseClient(userver::components::ComponentList& component_list);

}  // namespace warehouse_api

template <>
inline constexpr bool userver::components::kHasValidate<warehouse_api::WarehouseClient> = true;
