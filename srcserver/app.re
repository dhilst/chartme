open Express;
type pJSON = {
    version: string,
};
type path;
type cookieParser = unit => handler;
[@bs.module "path"] [@bs.val] external path : path = "path";
[@bs.module "./package.json"] [@bs.val] external pJSON : pJSON = "pJSON";
[@bs.module "cookie-parser"] [@bs.val] external cookieParser : cookieParser= "cookieParser";

[@bs.send] external join : (path, string, string) => string = "join";
[@bs.val] external __dirname: string = "__dirname";
[@bs.new] external newError : string => error = "Error";
// import routes index !?

// options
//   .version(pJSON.version)
//   .option('-l, --label <label>', 'Graph label')
//   .parse(process.argv);

let app = express();

Express.Options.(
options
  ->version(pJSON.version)
  ->option_("-l, --label <label>", "Graph label")
  )

app->set("views", path->join(__dirname, "views"));
app->set("view engine", "pug");

type urlencodedOptions = { .
  extended: bool,
};

app->use(logger("dev"));
app->use(bodyparser->json);
app->use(bodyparser->urlencoded({ "extended": false }));
app->use(cookieParser());
app->use(express->static("public"));
app->use(express->static("dist"));
app->usePrefix("/javascripts", express->static(path->join(__dirname, "/node_modules/jquery/dist/")));
app->usePrefix("/javascripts", express->static(path->join(__dirname, "/node_modules/chart.js/dist/")));
app->usePrefix("/javascripts", express->static(path->join(__dirname, "./node_modules/socket.io-client/dist/")));

app->usePrefix("/", Index_bs.router);

app->use((_req, _res, next) => {
  let err = newError("Not Found");
  err.status = Some(404);

  next(Some(err));
});

app->useError((err, _req, res, _next) => {
  res.locals.message = err.message;
  res.locals.error = err;
  let status = switch (err.status) {
    | Some(n) => n;
    | None => 500;
  }
  res.status(status);
  res.render("error");
});
