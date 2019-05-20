module Restaurants = [%graphql
  {|
    query {
      restaurants {
        id
        name
      }
    }
  |}
];

module RestaurantsQuery = Apollo.CreateQuery(Restaurants);

[@react.component]
let make = () => {
  let query = RestaurantsQuery.useQuery();

  switch (query.result) {
  | Loading => <div> {React.string("Loading...")} </div>
  | Error(error) => <div> {React.string(error##message)} </div>
  | Data(response) =>
    <ul>
      {response##restaurants
       ->Belt.Array.map(restaurant =>
           <li key={restaurant##id}> {React.string(restaurant##name)} </li>
         )
       ->React.array}
    </ul>
  };
};
