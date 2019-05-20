[@bs.module "@apollo/react-hooks"]
external _useQuery: ReasonApolloTypes.queryString => 'a = "useQuery";

[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

let useQuery = (~query) => {
  let graphqlQueryAST = gql(. query##query);
  Js.log(graphqlQueryAST);

  let result = _useQuery(graphqlQueryAST);
  Js.log(result);
  result;
  //   switch (
  //     result->loadingGet,
  //     result->errorGet,
  //     result->dataGet,
  //     result->fetchErrorGet,
  //     result->httpErrorGet,
  //   ) {
  //   | (true, _, _, _, _) => Loading
  //   | (false, false, Some(response), _, _) => Data(response |> query##parse)
  //   | (false, true, None, Some(fetchError), None) =>
  //     Error(fetchError->messageGet)
  //   | (false, true, None, None, Some(httpError)) => Error(httpError->bodyGet)
  //   | _ => Error("Something went wrong")
  //   };
};

module Provider = {
  [@bs.module "@apollo/react-hooks"] [@react.component]
  external make:
    (~client: ApolloClient.generatedApolloClient, ~children: React.element) =>
    React.element =
    "ApolloProvider";
};
