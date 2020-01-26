type window = {. [@bs.set] "onload": unit => unit};
type document;
type element;
type chart;
type jQueryElement = {length: int};
type socket;

type datasets = {
  mutable label: string,
  mutable data: array(int),
};

type data = {
  mutable labels: array(string),
  mutable datasets: array(datasets),
};

type chartConfigOptions = {
  resposive: bool,
  maintainAspectRatio: bool,
};

type chartConfig = {
  [@bs.as "type"]
  type_: string,
  data,
  options: chartConfigOptions,
};

[@bs.val] external window: window = "window";
[@bs.val] external document: document = "document";
[@bs.val] [@bs.scope "document"]
external getElementById: string => element = "getElementById";
[@bs.new] external chart: (jQueryElement, chartConfig) => chart = "Chart";
[@bs.send] external chartUpdate: chart => unit = "update";
[@bs.val] external jQuerySelector: string => jQueryElement = "jQuery";
[@bs.val] external io: unit => socket = "io";
[@bs.send] external socketon: (socket, string, int => unit) => unit = "on";

let jQuerySelectorSafe = (selector: string): option(jQueryElement) => {
  let element = jQuerySelector(selector);
  switch (element.length) {
  | n when n > 0 => Some(element)
  | _ => None
  };
};

exception ElementNotFound(string);

let windowOnLoad = () => {
  Js.log("hello");
  let data = {labels: [||], datasets: [|{label: "", data: [||]}|]};
  let key = "#mychart";
  let canvas =
    switch (jQuerySelectorSafe(key)) {
    | Some(e) => e
    | None => raise(ElementNotFound(key))
    };
  Js.log(canvas);
  let graph =
    chart(
      canvas,
      {
        type_: "line",
        data,
        options: {
          resposive: true,
          maintainAspectRatio: false,
        },
      },
    );

  Js.Promise.(
    Fetch.fetch("/options")
    |> then_(Fetch.Response.json)
    |> then_((json: Js.Json.t) =>
         {
           Js.Json.(
             switch (classify(json)) {
             | JSONObject(obj) =>
               switch (Js.Dict.get(obj, "label")) {
               | Some(label) =>
                 switch (classify(label)) {
                 | JSONString(label) =>
                   data.datasets[0].label = label;
                   chartUpdate(graph);
                 | _ => ()
                 }
               | _ => ()
               }
             | _ => ()
             }
           );
         }
         |> resolve
       )
  );

  let socket = io();
  socketon(
    socket,
    "newdata",
    (newData: int) => {
      Js.log2("New data", newData);
      Js.Array.push("", data.labels) |> ignore;
      Js.Array.push(newData, data.datasets[0].data) |> ignore;
      chartUpdate(graph);
    },
  );
  ();
};

window##onload #= windowOnLoad;
