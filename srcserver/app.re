type app;
type error = {
  mutable status: option(int),
  mutable message: string,
  code: string,
};
type path;
type resLocals = {
  mutable message: string,
  mutable error,
};
type res = {locals: resLocals};
type req;
type next = option(error) => unit;
type handler = (req, res, next) => unit;
type cookieParser = unit => handler;
[@bs.module] [@bs.val] external path: path = "path";
[@bs.module] [@bs.val] external cookieParser: cookieParser = "cookie-parser";
type express = unit => app;
[@bs.module] external express: express = "express";
[@bs.send] external set: (app, string, string) => unit = "set";
type errorHandler = (error, req, res, next) => unit;
[@bs.send] external useError: (app, errorHandler) => unit = "use";
[@bs.send] external use: (app, handler) => unit = "use";
[@bs.send] external usePrefix: (app, string, handler) => unit = "use";
[@bs.send] external static: (express, string) => handler = "static";
[@bs.send] external status: (res, int) => unit = "status";
type router;
[@bs.send] external router: express => router = "Router";

[@bs.send] external join: (path, string, string) => string = "join";
[@bs.val] external __dirname: string = "__dirname";
[@bs.new] external newError: string => error = "Error";

type options;
[@bs.module] external options: options = "commander";
[@bs.send] external version: (options, string) => options = "version";
[@bs.send] external option_: (options, string, string) => options = "option";
[@bs.send]
external parse: (options, array(string)) => Js.t(options) = "parse";

[@bs.module] external logger: string => handler = "morgan";
type bodyparser;
[@bs.module] external bodyparser: bodyparser = "body-parser";
[@bs.send] external json: bodyparser => handler = "json";
[@bs.send] external urlencoded: (bodyparser, 'a) => handler = "json";

let app: app = express();
[@bs.module "fs"] external readFileSync: string => string = "readFileSync";

let packageVersion =
  switch (
    readFileSync("package.json") |> Js.Json.parseExn |> Js.Json.classify
  ) {
  | Js.Json.JSONObject(o) =>
    switch (Js.Dict.get(o, "version")) {
    | Some(v) =>
      switch (Js.Json.classify(v)) {
      | Js.Json.JSONString(v) => v
      | _ => raise(Failure("Can't parse package.json"))
      }
    | _ => raise(Failure("Can't parse package.json"))
    }
  | _ => raise(Failure("Can't parse package.json"))
  };

options
->version(packageVersion)
->option_("-l, --label <label>", "Graph label");

app->set("views", path->join(__dirname, "views"));
app->set("view engine", "pug");

type urlencodedOptions = {. extended: bool};

app->use(logger("dev"));
app->use(bodyparser->json);
app->use(bodyparser->urlencoded({"extended": false}));
app->use(cookieParser());
app->use(express->static("../public"));
app->use(express->static("../dist"));
app
->usePrefix(
    "/javascripts",
    express->static(path->join(__dirname, "/../node_modules/jquery/dist/")),
  );
app
->usePrefix(
    "/javascripts",
    express->static(path->join(__dirname, "/node_modules/chart.js/dist/")),
  );
app
->usePrefix(
    "/javascripts",
    express
    ->static(path->join(__dirname, "./node_modules/socket.io-client/dist/")),
  );

[@bs.send] external get: (router, string, handler) => unit = "get";
[@bs.send]
external render: (res, string, option(Js.Json.t)) => unit = "render";
[@bs.send] external setHeader: (res, string, string) => unit = "setHeader";
[@bs.send] external send: (res, string) => unit = "send";
[@bs.send] external appGet: (app, string) => Js.Json.t = "appGet";
[@bs.send] external usePrefix: (app, string, router) => unit = "use";

let router = express->router;
router
->get("/", (_, res, next) => {
    let options = Js.Dict.empty();
    Js.Dict.set(options, "title", Js.Json.string("Chart me!"));
    res->render("index", Some(Js.Json.object_(options)));
    next(None);
  });

router
->get("/options", (_, res, next) => {
    res->setHeader("Content-Type", "application/json");
    res->send(Js.Json.stringify(app->appGet("options")));
    next(None);
  });
app->usePrefix("/", router);

app
->use((_req, _res, next) => {
    let err = newError("Not Found");
    err.status = Some(404);

    next(Some(err));
  });

app
->useError((err, _req, res, _next) => {
    res.locals.message = err.message;
    res.locals.error = err;
    let s =
      switch (err.status) {
      | Some(n) => n
      | None => 500
      };
    res->status(s);
    res->render("error", None);
  });
