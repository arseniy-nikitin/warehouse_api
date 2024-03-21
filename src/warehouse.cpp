#include "warehouse_client.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace warehouse_api {

int WarehouseClient::CreateWarehouse(std::string address, std::string area) {
	handlers::api::CreateWarehouseRequest request;
	request.set_address(std::move(address));
	request.set_area(std::move(area));

	// Deadline must be set manually for each RPC
  	auto context = std::make_unique<grpc::ClientContext>();
  	context->set_deadline(
      		userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

	// Initiate the RPC. No actual actions have been taken thus far besides
  	// preparing to send the request.
  	auto stream = client_.CreateWarehouse(request, std::move(context));

	// Complete the unary RPC by sending the request and receiving the response.
 	// The client should call `Finish` (in case of single response) or `Read`
  	// until `false` (in case of response stream), otherwise the RPC will be
  	// cancelled.
  	handlers::api::HelloResponse response = stream.Finish();

	return response.code();
}

warehouse::Warehouse WarehouseClient::GetWarehouse(int id) {
	handlers::api::CreateWarehouseRequest request;
	request.set_id(std::move(id));
	
	auto context = std::make_unique<grpc::ClientContext>();
  	context->set_deadline(
      		userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

 	// Call the gRPC GetWarehouse method on the warehouse service
  	warehouse::Warehouse response;
  	auto status = stub_->GetWarehouse(&context, request, &response);

  	// Check the gRPC status code and return the response message
  	if (status.ok()) {
    		return response;
  	} else {
    		throw std::runtime_error("Failed to get warehouse: " + status.error_message());
  	}
}

std::vector<warehouse::Warehouse> WarehouseClient::ListWarehouses(int offset, int limit) {
  // Create a gRPC context with a deadline of 10 seconds
  grpc::ClientContext context;
  context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(10));

  // Create a request message with the warehouse ID filter
  warehouse::GetWarehousesRequest request;
  request.set_id(id);

  // Call the gRPC GetWarehouses method on the warehouse service
  warehouse::GetWarehousesResponse response;
  auto status = stub_->GetWarehouses(&context, request, &response);

  // Check the gRPC status code and return the response message
  if (status.ok()) {
    return response.warehouses();
  } else {
    throw std::runtime_error("Failed to get warehouses: " + status.error_message());
  }
}

// Provide documentation for logging ???
userver::yaml_config::Schema WarehouseClient::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: >
    A user-defined wrapper around api::WarehouseServiceClient that provides
    a interface for creating, retrieving, and retrieving multiple
    warehouse members.
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            The endpoint for the WarehouseService service.
    create_warehouse_endpoint:
        type: string
        description: >
            The endpoint for the Create method.
    get_warehouse_endpoint:
        type: string
        description: >
            The endpoint for the Get method.
    list_warehouse_endpoint:
        type: string
        description: >
            The endpoint for the Get List method.
)");
}

// Initialize module of our client
void AppendWarehouseClient(userver::components::ComponentList& component_list) {
  component_list.Append<WarehouseClient>();
}

}  // namespace warehouse_api
