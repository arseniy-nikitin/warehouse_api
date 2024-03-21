DROP SCHEMA IF EXISTS warehouse_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS warehouse_schema;

CREATE TABLE IF NOT EXISTS warehouse.warehouses (
	id SERIAL PRIMARY KEY,
	address VARCHAR(255) NOT NULL,
	area VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS warehouse.products (
	id SERIAL PRIMARY KEY,
	warehouse_id INTEGER REFERENCES warehouses(id),
	name VARCHAR(255) NOT NULL,
	weight NUMERIC(10, 2) NOT NULL,
	width NUMERIC(10, 2) NOT NULL,
	len NUMERIC(10, 2) NOT NULL,
	height NUMERIC(10, 2) NOT NULL,
	is_fragile BOOLEAN NOT NULL
);

CREATE TABLE IF NOT EXISTS warehouse.couriers (
	id SERIAL PRIMARY KEY,
	name VARCHAR(255) NOT NULL,
	area VARCHAR(50) NOT NULL,
	courier_type VARCHAR(50) NOT NULL,
	working_hours_start TIME NOT NULL,
	working_hours_end TIME NOT NULL
);

CREATE TABLE IF NOT EXISTS warehouse.orders (
	id SERIAL PRIMARY KEY,
	product_id INTEGER REFERENCES products(id),
	courier_id INTEGER REFERENCES couriers(id),
	customer_name VARCHAR(255) NOT NULL,
	customer_address VARCHAR(255) NOT NULL,
	customer_phone VARCHAR(20) NOT NULL,
	customer_email VARCHAR(255),
	payment_method VARCHAR(50) NOT NULL,
	order_status VARCHAR(50) NOT NULL,
	delivery_date DATE NOT NULL,
	delivery_period_start TIME NOT NULL,
	delivery_period_end TIME NOT NULL,
	area VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS warehouse.delivery_reports (
	id SERIAL PRIMARY KEY,
	order_id INTEGER REFERENCES orders(id),
	delivery_timestamp TIMESTAMP NOT NULL,
	description TEXT
);
