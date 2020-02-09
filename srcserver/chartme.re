open App;

type server;
[@bs.module "http"] external createServer : app => server = "createServer";
[@bs.send] external listen: (server, string) => unit = "listen";
[@bs.send] external onError : (server, string, error => unit) => unit = "on";
[@bs.send] external on : (server, string, unit => unit) => unit = "on";
[@bs.module] external debugModule : string => (string => unit) = "debug";
let debug = debugModule("chartme:server");

type socketio;
[@bs.module] external socketio : server => socketio = "socket.io";
[@bs.send] external onSocket: (socketio, string, socketio => unit) => unit = "on";
[@bs.send] external emit : (socketio, string, int) => unit = "emit";
[@bs.val] external parseInt: string => int = "parseInt";
[@bs.module "process"] [@bs.scope"stdin"] external processStdinOn : (string, string => unit) => unit  = "on";

// it cant be a path for a unix socket
// this is why is a string
let port: string = "3000";
app->set("port", port);


let server = createServer(app);
let io = socketio(server);

server->listen(port);
server->onError("error", error => raise(Failure("oh no " ++ error.code)));
server->on("listening", () => {
  debug("listenting on port 3000");
  Js.log("Server started ...")
});

io->onSocket("connection", socket => {
  processStdinOn("data", data => {
    let data = parseInt(data);
    Js.log(data);
    socket->emit("newdata", data)
  });
});
