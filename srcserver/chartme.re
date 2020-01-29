open App;

type server;
[@bs.module "http"] external createServer : app => server = "createServer";
[@bs.send] external listen: (server, string) => unit = "listen";
[@bs.send] external onError : (server, string, error => unit) => unit = "on";
[@bs.send] external on : (server, string, unit => unit) => unit = "on";

type socketio;
[@bs.module] external socketio : server => socketio = "socket.io";

// it cant be a path for a unix socket
// this is why is a string
let port: string = "3000";
app->set("port", port);


let server = createServer(app);
let io = socketio(server);

server->listen(port);
server->onError("error", error => raise(Failure("oh no " ++ error.code)));
server->on("listening", () => Js.log("Server started ..."));
