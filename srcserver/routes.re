open Express;

let router = express->router;
router
->get("/", (_, res, next) => {
    let options = Js.Dict.empty();
    Js.Dict.set(options, "title", Js.Json.string("Chart me!"));
    res->render("index", Js.Json.object_(options));
    next(None);
  });

router
->get("/options", (_, res, next) => {
    open App;
    res->setHeader("Content-Type", "application/json");
    res->send(Js.Json.stringify(app->appGet("options")));
    next(None);
  });
