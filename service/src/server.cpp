// Server.cpp : http server for sending and recieving files to pass to face detection
//

#include "server.h"
#include "faces.h"
namespace fs = boost::filesystem;

template < typename RESP >
RESP
init_resp(RESP resp)
{
	resp.append_header(restinio::http_field::server, "RESTinio sample server /v.0.2");
	resp.append_header_date_field();

	return resp;
};

using router_t = restinio::router::express_router_t<>;

auto create_request_handler(fs::path p)
{
	auto router = std::make_unique< router_t >();

	router->http_get(
		"/files",
		[](auto req, auto) {
		init_resp(req->create_response())
			.append_header(restinio::http_field::content_type, "text/html; charset=utf-8")
			.set_body(
				"<html>\r\n"
				"  <head><title>Findfaces OpenCV demo</title></head>\r\n"
				"  <body>\r\n"
				"    <center><h1>Findfaces OpenCV demo</h1></center>\r\n"
				"    <p>\r\n"
				"This is a demo C++ OpenCV application that exposes a service for "
				"recieving an image to find faces in which is saved to a new file with "
				"the faces circled. See the readme.md with the source for more info.\r\n"
				"    <p>\r\n"
				"  </body>\r\n"
				"</html>\r\n")
			.done();

		return restinio::request_accepted();
	});

	router->http_get(
		"/files/:file",
		[p](auto req, auto params) {
		const std::string filename = restinio::cast_to<std::string>(params["file"]);
		auto fn = p;
		fn /= filename;

		if (fs::exists(fn)) {
			init_resp(req->create_response())
				.append_header(restinio::http_field::content_type, "text/plain; charset=utf-8")
				.set_body( restinio::sendfile(fn.string()))
				.done();
		}
		else {
			return
				req->create_response(404, "Not found")
				.connection_close()
				.done();
		}

		return restinio::request_accepted();
	});

	router->http_put(
		"/files",
		[p](auto req, auto) {
		const auto qp = restinio::parse_query(req->header().query());
		const std::string filename = restinio::cast_to<std::string>(qp["submit"]);
		auto in = p;
		auto out = p;
		in /= filename;
		out /= "faces" + filename;
		fs::ofstream ofs;
		ofs.open(in, std::ios_base::out | std::ios_base::binary);
		ofs << req->body();
		ofs.close();

		if (qp.has( "submit" )) {
			init_resp(req->create_response())
				.append_header(restinio::http_field::content_type, "text/json; charset=utf-8")
				.set_body(fmt::format("File {} accepted", qp["submit"]))
				.done();
		}

        Faces faces;
		faces.find(in.string(), out.string());
        		
		return restinio::request_accepted();
	});

	router->non_matched_request_handler(
		[](auto req) {
		return
			req->create_response(404, "Not found")
			.connection_close()
			.done();
	});

	return router;
}

int main()
{
	fs::path path = fs::current_path();
	std::cout << "using path " << path << "\n";
	path /= "files";
	if (fs::exists(path)) {
		std::cout << "directory exists\n";
	}
	else {
		fs::create_directory(path);
		std::cout << "directory created\n";
	}

	try
	{
		using traits_t =
			restinio::traits_t<
			restinio::asio_timer_manager_t,
			restinio::single_threaded_ostream_logger_t,
			router_t >;

		restinio::run(
			restinio::on_this_thread<traits_t>()
			.port(4000)
			.address("0.0.0.0")
			.request_handler(create_request_handler(path)));
	}
	catch (const std::exception & ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		return 1;
	}

	return 0;
}
