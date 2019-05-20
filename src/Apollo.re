module CreateQuery = (Config: ReasonApolloTypes.Config) => {
  [@bs.module "@apollo/react-hooks"]
  external _useQuery: ReasonApolloTypes.queryString => 'a = "useQuery";

  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

  type apolloErrorExtensions = {. "code": Js.Nullable.t(string)};

  type graphqlError = {
    .
    "message": string,
    "name": Js.Nullable.t(string),
    "extensions": Js.Nullable.t(apolloErrorExtensions),
    "locations": Js.Nullable.t(array(string)),
    "path": Js.Nullable.t(array(string)),
    "nodes": Js.Nullable.t(array(string)),
  };

  type apolloError = {
    .
    "message": string,
    "graphQLErrors": Js.Nullable.t(array(graphqlError)),
    "networkError": Js.Nullable.t(string),
  };

  type queryResponse('a) =
    | Loading
    | Error(apolloError)
    | Data('a);

  type response = queryResponse(Config.t);

  [@bs.deriving abstract]
  type renderPropObjJS = {
    loading: bool,
    data: Js.Nullable.t(Js.Json.t),
    error: Js.Nullable.t(apolloError),
    refetch: Js.Null_undefined.t(Js.Json.t) => Js.Promise.t(renderPropObjJS),
    networkStatus: Js.Nullable.t(int),
    variables: Js.Null_undefined.t(Js.Json.t),
  };

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool,
    refetch: option(Js.Json.t) => Js.Promise.t(response),
    networkStatus: option(int),
  };

  let apolloDataToVariant: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData->loadingGet,
        apolloData->dataGet |> Js.Nullable.toOption,
        apolloData->errorGet |> Js.Nullable.toOption,
      ) {
      | (true, _, _) => Loading
      | (false, Some(response), _) => Data(Config.parse(response))
      | (false, _, Some(error)) => Error(error)
      | (false, None, None) =>
        Error({
          "message": "No data",
          "graphQLErrors": Js.Nullable.null,
          "networkError": Js.Nullable.null,
        })
      };

  let convertJsInputToReason = (apolloData: renderPropObjJS) => {
    result: apolloData |> apolloDataToVariant,
    data:
      switch (apolloData->dataGet |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) =>
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
    error:
      switch (apolloData->errorGet |> Js.Nullable.toOption) {
      | Some(error) => Some(error)
      | None => None
      },
    loading: apolloData->loadingGet,
    refetch: variables =>
      apolloData->(refetchGet(variables |> Js.Nullable.fromOption))
      |> Js.Promise.then_(data =>
           data |> apolloDataToVariant |> Js.Promise.resolve
         ),
    networkStatus: apolloData->networkStatusGet->Js.Nullable.toOption,
  };

  let useQuery = () => {
    let graphqlQueryAST = gql(. Config.query);

    let result = _useQuery(graphqlQueryAST);
    result |> convertJsInputToReason;
  };
};

module Provider = {
  [@bs.module "@apollo/react-hooks"] [@react.component]
  external make:
    (~client: ApolloClient.generatedApolloClient, ~children: React.element) =>
    React.element =
    "ApolloProvider";
};
