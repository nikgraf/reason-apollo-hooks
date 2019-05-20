/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="http://localhost:4000/graphql", ());

let apolloClient =
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=apolloClient>
    <ApolloHooks.Provider client=apolloClient> <App /> </ApolloHooks.Provider>
  </ReasonApollo.Provider>,
  "root",
);
