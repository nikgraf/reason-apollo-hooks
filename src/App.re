module RestaurantNames = [%graphql
  {|
    query {
      restaurants {
        id
        name
      }
    }
  |}
];

// module RestaurantNamesQuery = ReasonApollo.CreateQuery(RestaurantNames);

let query = RestaurantNames.make();

[@react.component]
let make = () => {
  let (count, setCount) = React.useState(() => 0);
  let result = ApolloHooks.useQuery(~query);
  Js.log2("weee" , result);

  <div>
    <p> {React.string(" clicked " ++ string_of_int(count) ++ " times")} </p>
    <button onClick={_ => setCount(_ => count + 1)}>
      {React.string("Click me")}
    </button>
  </div>;
  // <RestaurantNamesQuery>
  //   ...{({result}) =>
  //     switch (result) {
  //     | Loading => <div> {React.string("Loading")} </div>
  //     | Error(error) => <div> {React.string(error##message)} </div>
  //     | Data(response) =>
  //       <ul>
  //         {response##restaurants
  //          ->Belt.Array.map(restaurant =>
  //              <li key={restaurant##id}>
  //                {React.string(restaurant##name)}
  //              </li>
  //            )
  //          ->React.array}
  //       </ul>
  //     }
  //   }
  // </RestaurantNamesQuery>
};
